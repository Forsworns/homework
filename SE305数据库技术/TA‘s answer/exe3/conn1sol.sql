#SET autocommit = 0;
#begin;
#problem 1
drop table if exists test1;
create table if not exists test1 (
	test1_id int,
	PRIMARY KEY(test1_id)
) engine=MyISAM;
select connection_id();
insert into test1 values(1);
lock tables test1 read;
unlock table;
insert into test1 values(2);



#problem 2
drop table if exists test2;
create table if not exists test2 (
	test2_id int,
	PRIMARY KEY(test2_id)
) engine=MyISAM;
select connection_id();
insert into test2 values(1);
lock tables test2 read local;
insert into test2 values(2);

#problem 3
drop table if exists test3;
create table if not exists test3 (
	test3_id int,
	PRIMARY KEY(test3_id)
) engine=MyISAM;
select connection_id();
insert into test3 values(1);
lock tables test3 read local;
insert into test3 values(2);

#problem 4 
drop table if exists test4;
create table if not exists test4 (
	test4_id int,
	PRIMARY KEY(test4_id)
) engine=MyISAM;
select connection_id();
insert into test4 values(1);
lock tables test4 read local;
unlock table;
insert into test4 values(2);

#problem 5
drop table if exists test5;
create table if not exists test5 (
	test5_id int,
	PRIMARY KEY(test5_id)
) engine=MyISAM;
select connection_id();
insert into test5 values(1);
lock tables test5 write;
unlock table;
insert into test5 values(2);

#problem 6
drop table if exists test6_1;
drop table if exists test6_2;
create table if not exists test6_1 (
	test6_1_id int,
	PRIMARY KEY(test6_1_id)
) engine=MyISAM;
create table if not exists test6_2 (
	test6_2_id int,
	PRIMARY KEY(test6_2_id)
) engine=MyISAM;
select connection_id();
insert into test6_1 values(1);
lock tables test6_1 write; 
unlock table;
insert into test6_2 values(2);

 

 