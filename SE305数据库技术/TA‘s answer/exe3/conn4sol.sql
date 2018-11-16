 set autocommit = 0;
	set session transaction isolation level read uncommitted;
#problem 1
drop procedure if exists problem1;
delimiter //
create procedure problem1()
begin
	select connection_id();
	select * from test1 where test1_id = '1' for update;
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
	select * from test2 where val = '3' for update;
end
//
delimiter ; 
call problem2;

#problem 3
drop procedure if exists problem3;
delimiter //
create procedure problem3()
begin
	select connection_id();
	update test3 set val = 100 where test3_id = 1;
    select * from test3 where test3_id = 1;
end
//
delimiter ; 
call problem3;

#problem 4 
drop procedure if exists problem4;
delimiter //
create procedure problem4()
begin
	select connection_id();
    select * from test4 where test4_id = 3;# lock in share mode;
end
//
delimiter ; 
call problem4;

#problem 5
drop procedure if exists problem5;
delimiter //
create procedure problem5()
begin
	select connection_id();
    select * from test5 where col = 7; #lock in share mode;
end
//
delimiter ; 
call problem5;

#problem 6
drop procedure if exists problem6;
delimiter //
create procedure problem6()
begin
	select connection_id();
    select count(*) from test6 where val = 3;
end
//
delimiter ; 
call problem6;
