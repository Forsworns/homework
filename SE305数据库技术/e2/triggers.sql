
SET SQL_SAFE_UPDATES = 0;
delimiter //
drop procedure if exists `refuse`//
create procedure refuse(in s varchar(2000))
begin
	signal sqlstate '45000'
	set message_text = s;
end//

drop procedure if exists `sp_split`//
create procedure sp_split(in c1 varchar(2000),in split1 varchar(20))
begin
	create temporary table if not exists temp_split
	(
	   col varchar(20)
	);
	delete from temp_split;
	while(instr(c1,split1)<>0) DO
		insert temp_split(col) values (substring(c1,1,instr(c1,split1)-1));
		set c1 = insert(c1,1,instr(c1,split1),'');
	end while;
	insert temp_split(col) values (c1); 
end;
//



drop procedure if exists `checkTypeUnit`//
create procedure checkTypeUnit(in s varchar(45))
begin
	declare i int;
	declare j int;
    declare listCount int;
    declare typeName varchar(16);
	call sp_split(s,'，');
    set listCount = (
			select count(*) 
            from temp_split
        );
	set i = 1;
	while i<=listCount do
		set j = i-1;
		set typeName =(
			select * 
			from temp_split 
			limit j,i
		);
		set i = i + 1;
        if typeName != '食物' and typeName != '衣服' and typeName != '娱乐' then
			call refuse(concat(typeName,'不是允许的类别'));
        end if;
	end while;
end;
//


drop trigger if exists checkCustomerInsert//				
create trigger checkCustomerInsert before insert			
	on `mydb`.`customer` for each row
    begin
		if new.age > 80
        then
			call refuse('年龄过大');
        end if;
    end;
//

drop trigger if exists checkCustomerUpdate//				
create trigger checkCustomerUpdate before update			
	on `mydb`.`customer` for each row
    begin
		if new.age > 80
        then
			call refuse('年龄过大');
        end if;
    end;
//

drop trigger if exists checkSupplierInsert//				
create trigger checkSupplierInsert before insert			
	on `mydb`.`supplier` for each row
    begin
		if new.age > 80
        then
			call refuse('年龄过大');
        end if;
    end;
//

drop trigger if exists checkSupplierUpdate//				
create trigger checkSupplierUpdate before update			
	on `mydb`.`supplier` for each row
    begin
		if new.age > 80
        then
			call refuse('年龄过大');
        end if;
    end;
//

drop trigger if exists checkOrderListInsert//				
create trigger checkOrderListInsert before insert			
	on `mydb`.`orderList` for each row
    begin
		if (new.state!='未处理' and new.reason is not null) 
        then
			call refuse('订单状态变更逻辑错误');
        end if;
    end;
//

drop trigger if exists checkOrderListUpdate//				
create trigger checkOrderListUpdate before update			
	on `mydb`.`orderList` for each row
    begin # 订单逻辑变化的相关限制
		declare orderCur int;
		declare commodityCur varchar(45);
		declare amountReverse int;
		declare done int default false;
		declare cur cursor for 
		select `orderId` as orderCur,
			`commodityName` as commodityCur,
			`amount` as amountReverse
		from `commodityOrderList`;
		declare continue handler for not found set done=true; 
        
		if (new.state='已退货' and new.reason is null) 
			or (new.state!='已退货' and new.reason is not null) 
            or (old.state!='未处理' and new.state='已发货')
            or (old.state!='已发货' and new.state='已完成')
			or (old.state ='已完成' and new.state!='已退货' and new.reason is not null) 
        then
			call refuse('订单状态变更逻辑错误');
        end if;
        
        if new.state='已退货' then # 退货以后增加库存
			open cur;
				fetch  cur into orderCur,commodityCur,amountReverse;
				repeat
					update `commodity`
					set `amount` =`amount` + amountReverse
					where orderCur = new.`id` and commodityCur = `name`;
					fetch  cur into orderCur,commodityCur,amountReverse;
				until done end repeat;
			close cur;
        end if;
    end;
//



drop trigger if exists priceCalculate//
create trigger priceCalculate before insert				# 价格计算
	on `mydb`.`commodityorderlist` for each row
    begin
		set @points = (
			select points
            from `mydb`.`customer`
            where id = (
				select customer
                from `mydb`.`orderList`
                where id = new.orderId
            )
        );
		set @discountCustomer = (
			case
				when @points <= 100 then 0.9
                when @points between 101 and 500 then 0.8
                when @points >= 501  then 0.7
			end
        );
        set @discountSupplier = (
			select discount
            from `mydb`.`commodity`
            where `name` = new.commodityName
        );
        set @unitPrice = (
			select price
            from `mydb`.`commodity`
            where `name` = new.commodityName
        );
        set new.price = new.amount * @unitPrice * @discountCustomer * @discountSupplier;
    end;
//

drop trigger if exists checkAmount//
create trigger checkAmount before insert			# 购买前先检查库存是否充足，充足则更新
	on `mydb`.`commodityorderlist` for each row
    begin
		set @amount = (
			select `amount`
            from `mydb`.`commodity`
            where `name` = new.`commodityName`
        );
		if new.`amount` > @amount then
			call refuse('库存不足');
		else
			update `mydb`.`commodity`
		set `amount` = `amount` - new.`amount`
			where `name` = new.`commodityName`;
       end if;
    end;
//


drop trigger if exists pointsIncrease//
create trigger pointsIncrease after insert			# 评价后积分增长
	on `mydb`.`comment` for each row
	begin
		update `mydb`.`customer`
        set `points` = `points` + 5
		where `id` = (
			select `customer`
			from `mydb`.`orderList`
			where `id` = new.orderListId
        ) and `points` is not null;
    end;
//

drop trigger if exists checkCommentInsert//				
create trigger checkCommentInsert before insert			
	on `mydb`.`comment` for each row
    begin 
		if new.score<0 or new.score>5 then
			call refuse('评价分数错误');
        end if;
    end;
//

drop trigger if exists checkCommentUpdate//				
create trigger checkCommentUpdate before update			
	on `mydb`.`comment` for each row
    begin 
		if new.score is not null and (new.score<0 or new.score>5) then
			call refuse('评价分数错误');
        end if;
    end;
//

drop trigger if exists checkCommodityInsert//				
create trigger checkCommodityInsert before insert			
	on `mydb`.`commodity` for each row
    begin 
		if new.discount<0 or new.discount>1 then
			call refuse('折扣错误');
        end if;
    end;
//

drop trigger if exists checkCommodityUpdate//				
create trigger checkCommodityUpdate before update			
	on `mydb`.`commodity` for each row
    begin 
		if new.discount is not null and (new.discount<0 or new.discount>1) then
			call refuse('折扣错误');
        end if;
    end;
//

drop trigger if exists typeValifyInsert// 
create trigger typeValifyInsert before insert      		# 验证输入的商品种类符合规范，但是需要用逗号分隔
	on `mydb`.`commodity` for each row
    begin
        call checkTypeUnit(new.typeName);
    end;
//

delimiter ;