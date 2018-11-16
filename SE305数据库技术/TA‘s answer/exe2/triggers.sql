delimiter $$
drop  procedure if exists insert_vendor_account$$
create procedure insert_vendor_account()
begin
	declare num int;
	declare rand_name varchar(45);
    declare rand_gender varchar(6);
    declare rand_age smallint;
    declare rand_phone char(8);
    declare rand_account varchar(16);
    declare rand_password varchar(16);

    set num=1;
    while num <= 10 do
		set rand_name = SUBSTRING( MD5(RAND()) ,1,45);
		if floor(rand() * 2) = 0 then
			set rand_gender = 'Female';
		else
			set rand_gender = 'Male';
		end if;
            
		set rand_age = floor(20 + rand() * 20);
        set rand_phone = cast(floor(10000000 + rand() * 89999999) as char(8));
        
        set rand_account = SUBSTRING( MD5(RAND()) ,1,16);
        set rand_password = SUBSTRING( MD5(RAND()) ,1,16);
        
		insert into vendor_account(`name`,gender,age,phone,`account`,`password`) values(rand_name,rand_gender,rand_age,rand_phone, rand_account, rand_password);
        set num = num+1;
	end while;
end
$$

drop  procedure if exists insert_customer_account$$
create procedure insert_customer_account()
begin
	declare num int;
	declare rand_name varchar(45);
    declare rand_gender varchar(6);
    declare rand_age smallint;
    declare rand_phone char(8);
    
        declare rand_account varchar(16);
    declare rand_password varchar(16);

    set num=1;
    while num <= 100 do
		set rand_name = SUBSTRING( MD5(RAND()) ,1,45);
		if floor(rand() * 2) = 0 then
			set rand_gender = 'Female';
		else
			set rand_gender = 'Male';
		end if;
            
		set rand_age = floor(20 + rand() * 20);
        set rand_phone = cast(floor(10000000 + rand() * 89999999) as char(8));
        
		set rand_account = SUBSTRING( MD5(RAND()) ,1,16);
        set rand_password = SUBSTRING( MD5(RAND()) ,1,16);
        
		insert into customer_account(`name`,gender,age,phone,`account`,`password`) values(rand_name,rand_gender,rand_age,rand_phone, rand_account, rand_password);
        set num = num+1;
	end while;
end
$$

 
 

drop  procedure if exists insert_product$$
create procedure insert_product()
begin

	declare id int;
    declare num int;
    
    insert ignore into category values('cloth'),('food'),('entertainment');
    
    
    set id = 1;
    while id <= 10 do

        set num = 1;
        while num <= 4 do
        
			set @current_category = 'cloth';
			set @product_name = concat(id, '_', @current_category, '_', num);
            
			insert into product (`name`, production_date, expiration_date, price, `count`, stock, vendor_account_id)
			values (
				@product_name, 
				date_sub( CURRENT_TIMESTAMP(), interval floor(rand()*100) day), 
				date_add( CURRENT_TIMESTAMP(), interval floor(100+rand()*100) day), 
				1+floor(rand()*1000), rand(), 
				1+floor(rand()*100), 
				id
            );
            
            insert into product_has_category values ((select product.id from product where product.`name` = @product_name), @current_category);
            
            set num = num+1;
            
		end while;
        
        set num = 1;
        while num <= 4 do
        
			set @current_category = 'food';
			set @product_name = concat(id, '_', @current_category, '_', num);
        
			insert into product(`name`, production_date, expiration_date, price, `count`, stock, vendor_account_id)
			values (
				@product_name, 
				date_sub( CURRENT_TIMESTAMP(), interval floor(rand()*100) day), 
				date_add( CURRENT_TIMESTAMP(), interval floor(100+rand()*100) day), 
				1+floor(rand()*1000), rand(), 
				1+floor(rand()*100), 
				id
            );
            insert into product_has_category values ((select product.id from product where product.`name` = @product_name), @current_category);

            set num = num+1;
		end while;
        
        set num = 1;
        while num <= 4 do
        
			set @current_category = 'entertainment';
			set @product_name = concat(id, '_', @current_category, '_', num);
        
			insert into product(`name`, production_date, expiration_date, price, `count`, stock, vendor_account_id)
			values (
				@product_name, 
				date_sub( CURRENT_TIMESTAMP(), interval floor(rand()*100) day), 
				date_add( CURRENT_TIMESTAMP(), interval floor(100+rand()*100) day), 
				1+floor(rand()*1000), rand(), 
				1+floor(rand()*100), 
				id
            );
            
            insert into product_has_category values ((select product.id from product where product.`name` = @product_name), @current_category);
            
            set num = num+1;
		end while;
        
        
        set id = id+1;
    end while;

end
$$

drop trigger if exists trig_order_status$$
create trigger trig_order_status before insert on `order` for each row
begin
	set new.`status` = 'untreated';
end
$$

drop trigger if exists trig_initial_extra_count$$
create trigger trig_initial_extra_count after insert on customer_account for each row
begin


	insert into extra_count(customer_account_id, extra_count)
    values(new.id, 1);
end
$$

drop trigger if exists trig_VIP_extra_count$$
create trigger trig_VIP_extra_count after insert on points for each row
begin
	set @customer_point = new.`point`;

    if @customer_point < 0 then 
		set @extra_count = 1;
	elseif @customer_point <= 100 then
		set @extra_count = 0.9;
	elseif @customer_point <= 500 then
		set @extra_count = 0.8;
	else
		set @extra_count = 0.7;
	end if;

	update extra_count set extra_count.extra_count = @extra_count where extra_count.customer_account_id = new.customer_account_id;

end
$$


drop trigger if exists trig_extra_count$$
create trigger trig_extra_count after update on points for each row
begin
	set @customer_point = new.`point`;

    if @customer_point < 0 then 
		set @extra_count = 1;
	elseif @customer_point <= 100 then
		set @extra_count = 0.9;
	elseif @customer_point <= 500 then
		set @extra_count = 0.8;
	else
		set @extra_count = 0.7;
	end if;

	update extra_count set extra_count.extra_count = @extra_count where extra_count.customer_account_id = new.customer_account_id;
end
$$

drop trigger if exists trig_order_quantity$$
create trigger trig_order_quantity after insert on order_has_product for each row
begin
	if (new.quantity <= (select stock from product where product.id = new.product_id)) then
        
        update `order` set `status` = 'delivered' where id = new.order_id;
        update product set stock = stock - new.quantity where product.id = new.product_id;
    end if;
    
	set @product_price = (select price from product where id = new.product_id);
	set @product_count = (select `count` from product where id = new.product_id);
	set @product_customer_account_id = (select customer_account_id from `order` where id = new.product_id);
    
    
    
	set @extra_count = (select extra_count.extra_count from extra_count where customer_account_id = @product_customer_account_id);
    
    if @extra_count is null then set @extra_count = 1; end if;
    
    
    set @old_price = (select price from `order` where id = new.order_id);
    if @old_price is null then set @old_price = 0; end if;
    
    update `order` set price = @old_price + @product_price * @product_count * @extra_count * new.quantity where id = new.order_id;
    
end
$$

drop trigger if exists trig_return $$
create trigger trig_return after insert on return_order for each row
begin
	update product, order_has_product
    set product.stock = product.stock + order_has_product.quantity   
    where new.order_id = order_has_product.order_id and order_has_product.product_id = product.id;
	update `order`
    set `order`.status = 'returned'
    where new.order_id = `order`.id;
end
$$

drop trigger if exists trig_evaluation $$

create trigger trig_evaluation after insert on evaluation for each row
begin
    update points
    set `point` = `point` + 100
    where points.customer_account_id = (select customer_account_id from `order` where `order`.id = new.order_id);
    
    update `order`
    set `status` = 'completed'
    where id = new.order_id;
    
end
$$

DELIMITER ;   


call insert_vendor_account();
call insert_customer_account();
call insert_product();

