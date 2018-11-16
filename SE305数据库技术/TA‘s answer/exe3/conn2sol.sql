#problem 1
drop procedure if exists problem1;
delimiter //
create procedure problem1()
begin
	select connection_id();
	insert into test1 values(3);
	select * from test1;
end
//
delimiter ; 
call problem1;


#problem 2
drop procedure if exists problem2;
delimiter //
create procedure problem2()
begin
	select connection_id();
	insert into test2 values(3);
	select * from test2;
end
//
delimiter ; 
call problem2;


# problem  3
drop procedure if exists problem3;
delimiter //
create procedure problem3()
begin
	select connection_id();
	insert into test3 values(4);
	select * from test3;
end
//
delimiter ; 
lock tables test3 read;
unlock table;
insert into test3 values(3);
call problem3;


# problem  4
drop procedure if exists problem4;
delimiter //
create procedure problem4()
begin
	select connection_id();
	insert into test4 values(3);
	select * from test4;
end
//
delimiter ; 
lock tables test4 read; 
unlock table;
call problem4;

# problem 5
drop procedure if exists problem5;
delimiter //
create procedure problem5()
begin
	select connection_id();
	select * from test5;
end
//
delimiter ;   
call problem5;

# problem 6
drop procedure if exists problem6;
delimiter //
create procedure problem6()
begin
	select connection_id();
    insert into test6_2 values(3);
	select * from test6_2;
end
//
delimiter ;   
call problem6;

 

unlock table;