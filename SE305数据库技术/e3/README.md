## 第三次上机作业

* #### 表级锁

  该部分的重点在于使用`unlock tables`在适当的位置解开表级锁，同时要注意使用`local`修饰符可以使得其他会话可以获取当前会话的表执行插入。更改助教提供的代码后得到要求的输出，如下各图（代码见附页）

  ![](D:\文档\大三上\数据库技术\exe3\exe3\pic\1_1.png)

  

  ![1_2](D:\文档\大三上\数据库技术\exe3\exe3\pic\1_2.png)

  

  ![2_1](D:\文档\大三上\数据库技术\exe3\exe3\pic\2_1.png)

  

  ![2_2](D:\文档\大三上\数据库技术\exe3\exe3\pic\2_2.png)

  

  ![3_1](D:\文档\大三上\数据库技术\exe3\exe3\pic\3_1.png)

  

  ![3_2](D:\文档\大三上\数据库技术\exe3\exe3\pic\3_2.png)

  

  ![4_1](D:\文档\大三上\数据库技术\exe3\exe3\pic\4_1.png)

  

  ![4_2](D:\文档\大三上\数据库技术\exe3\exe3\pic\4_2.png)

  

  ![5_1](D:\文档\大三上\数据库技术\exe3\exe3\pic\5_1.png)

  

  ![5_2](D:\文档\大三上\数据库技术\exe3\exe3\pic\5_2.png)

  

  ![6_1](D:\文档\大三上\数据库技术\exe3\exe3\pic\6_1.png)

  

  ![6_2](D:\文档\大三上\数据库技术\exe3\exe3\pic\6_2.png)

* #### 行级锁

  该部分的重点在于如何合理使用两种锁（共享锁和排他锁），同时使用commit提交事务，使用rollback进行回滚。我在测试时发现如果使用commit则会释放添加过的锁。更改助教提供的代码后得到要求的输出，如下各图（代码见附页）

  ![](D:\文档\大三上\数据库技术\exe3\exe3\pic\7_1.png)

  

  ![7_2](D:\文档\大三上\数据库技术\exe3\exe3\pic\7_2.png)

  

  ![8_1](D:\文档\大三上\数据库技术\exe3\exe3\pic\8_1.png)

  

  ![8_2](D:\文档\大三上\数据库技术\exe3\exe3\pic\8_2.png)

  

  ![9_1](D:\文档\大三上\数据库技术\exe3\exe3\pic\9_1.png)

  

  ![9_2](D:\文档\大三上\数据库技术\exe3\exe3\pic\9_2.png)

  

  ![10_1](D:\文档\大三上\数据库技术\exe3\exe3\pic\10_1.png)

  

  ![10_2](D:\文档\大三上\数据库技术\exe3\exe3\pic\10_2.png)

  

  ![11_1](D:\文档\大三上\数据库技术\exe3\exe3\pic\11_1.png)

  

  ![11_2](D:\文档\大三上\数据库技术\exe3\exe3\pic\11_2.png)

  

  ![12_1](D:\文档\大三上\数据库技术\exe3\exe3\pic\12_1.png)

  

  ![12_2](D:\文档\大三上\数据库技术\exe3\exe3\pic\12_2.png)

* #### 测试默认事务隔离级别

  我最初想在workbench中测试，如下建立了一个student表用于测试

  ```
  set autocommit = 0;
  drop table if exists student;
  create table if not exists student(
  	stu_id int,
      score int
  );
  ```

  

  但是感到同时操作两个用户切换起来不方便，最后选择了使用Command Line Client结合测试。打开命令行客户端后，执行以下命令获取所需的数据库及对应的表

  ```mysql
  show databases;
  use xxx
  show tables;
  # check if there is the table we have created before in workbench
  ```

  

  首先我查询了默认情况下的事务隔离级别，可以使用`select @@transaction_isolation`获取。从图中可以得知，默认的级别为REPEATABLE-READ，同时可以看到，左侧的connection_id为45，右侧为44。

  ![](D:\文档\大三上\数据库技术\exe3\exe3\pic\初始为RR.png)

  

  接下来进行的是对幻读的测试，这里我在左侧首先开启了一个事务，接着右侧右侧开启了另一个事务，在右侧我插入了一个新的学号为5的学生，成绩为90，进行提交。左侧在事务内查询，未查到该值，提交。之后左侧再次查询，查到了右侧刚刚添加的信息，这里发生了幻读。因为左侧在事务中读取时无法读到右侧新增的信息，操作完成后才发现刚刚新增了一个信息，如下图

  ![](D:\文档\大三上\数据库技术\exe3\exe3\pic\RR幻读.png)

  

  之后测试了是否可以重复读取，如下图。我首先在左侧开启了一个事务，获取了一次表中的信息，之后在右侧进行了更新操作，在左侧又进行了对应的查询，最后右侧提交，左侧再次查询。三次左侧的查询结果相同。这个测试证明了该级别下，右侧的数据的更改并提交不会对左侧事务内部的多次读取结果造成影响。

  ![](D:\文档\大三上\数据库技术\exe3\exe3\pic\RR可重复读.png)

  最后进行了脏读的测试。我在左侧开启了一个事务，右侧进行了更新操作，仍然是在每步操作及回滚后进行左侧的查询，可以看到左侧数据一直没有变化。测试表明该级别下右侧被回滚的更新操作不会对左侧产生影响。![](D:\文档\大三上\数据库技术\exe3\exe3\pic\RR不会脏读.png)

* #### 测试修改后的事务隔离级别

  之后我变更了事务隔离的级别，变更为了READ-UNCOMMITED。进行了相似的测试。测试结果表明该级别下不会读到脏数据，但是不可重复读，同时也会有幻读的现象，在最后一幅图的测试中，我发现右侧未提交的变化左侧都可以获取得到。

  ![](D:\文档\大三上\数据库技术\exe3\exe3\pic\切换RU.png)

  

  ![](D:\文档\大三上\数据库技术\exe3\exe3\pic\RU不会脏读.png)

  

  ![](D:\文档\大三上\数据库技术\exe3\exe3\pic\RU不可重复读.png)

  

  ![](D:\文档\大三上\数据库技术\exe3\exe3\pic\RU幻读.png)

* ### 结论

  综合之前的测试，我们可以得到表格中的结论。通过此次实验，我对事务管理和mySQL的设计及锁的表级锁和行级锁的应用有了了解。

  |   事务隔离级别   | 脏读 | 不可重复读 | 幻读 |
  | :--------------: | :--: | :--------: | :--: |
  | REPEATABLE-READ  |  否  |     否     |  是  |
  | READ-UNCOMMITTED |  否  |     是     |  是  |

* ### 代码

  四个修改后的文件按次序排列

  ```mysql
  #conn1.sql
  
   #problem 1
  unlock tables;
  drop table if exists test1;
  create table if not exists test1 (
  	test1_id int,
  	PRIMARY KEY(test1_id)
  ) engine=MyISAM;
  lock tables test1 write;
  select connection_id();
  insert into test1 values(1);
  insert into test1 values(2);
  unlock tables;
  
  unlock tables;
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
  unlock tables;
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
  unlock tables;
  drop table if exists test4;
  create table if not exists test4 (
  	test4_id int,
  	PRIMARY KEY(test4_id)
  ) engine=MyISAM;
  select connection_id();
  lock tables test4 write;
  insert into test4 values(1);
  insert into test4 values(2);
  unlock tables;
  
  #problem 5
  drop table if exists test5;
  create table if not exists test5 (
  	test5_id int,
  	PRIMARY KEY(test5_id)
  ) engine=MyISAM;
  select connection_id();
  lock tables test5 write;
  insert into test5 values(1);
  insert into test5 values(2);
  unlock tables;
  lock tables test5 read;
  
  #problem 6
  unlock tables;
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
  lock tables test6_1 read; 
  lock tables test6_2 write;
  insert into test6_2 values(2);
  unlock tables;
  lock tables test6_2 read local;
  ```

  ```mysql
  #conn2.sql
  
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
  lock tables test4 write;
  call problem4;
  unlock tables;
  
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
  lock tables test5 read;
  call problem5;
  unlock tables;
  
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
  ```

  ```mysql
  #conn3.sql
  
  #problem 1
  set autocommit = 0;
  drop table if exists test1;
  create table if not exists test1 (
  	test1_id int,
  	PRIMARY KEY(test1_id)
  ) engine=InnoDB;
  select connection_id();
  insert into test1 values(1);
  insert into test1 values(2);
  insert into test1 values(3);
  commit;
  select * from test1 where test1_id = 1 for update;
  
  
  
  #problem 2
  set autocommit = 0;
  drop table if exists test2;
  create table if not exists test2 (
  	test2_id int,
      val int,
  	PRIMARY KEY(test2_id)
  ) engine=InnoDB;
  select connection_id();
  insert into test2 values(1,2);
  insert into test2 values(2,3);
  insert into test2 values(3,4);
  commit;
  select test2_id from test2 where val = 2 lock in share mode;
  
  #problem 3
  set autocommit = 0;
  drop table if exists test3;
  create table if not exists test3 (
  	test3_id int,
      val int,
  	PRIMARY KEY(test3_id)
  ) engine=InnoDB;
  select connection_id();
  insert into test3 values(1,2);
  insert into test3 values(2,3);
  insert into test3 values(3,4);
  select test3_id from test3 where val = 2 lock in share mode;
  commit;
  
  
  #problem 4
  set autocommit = 0;
  drop table if exists test4;
  create table if not exists test4 (
  	test4_id int,
      val int,
      col int,
  	PRIMARY KEY(test4_id)
  ) engine=InnoDB;
  select connection_id();
  insert into test4 values(1,2,3);
  insert into test4 values(2,3,4);
  insert into test4 values(3,3,5);
  insert into test4 values(4,5,6);
  commit;
  select * from test4 where test4_id = 3 lock in share mode;
  update test4 set col= 10 where test4_id = 3;
  rollback;
  
  #problem 5
  set autocommit = 0;
  drop table if exists test5;
  create table if not exists test5 (
  	test5_id int,
      val int,
      col int,
  	PRIMARY KEY(test5_id)
  ) engine=InnoDB;
  select connection_id();
  insert into test5 values(1,2,3);
  insert into test5 values(2,3,4);
  commit;
  insert into test5 values(3,3,7);
  rollback;
  insert into test5 values(4,5,6);
  commit;
  select * from test5 where val = 3 lock in share mode;
  
  #problem 6
  set autocommit = 0;
  drop table if exists test6;
  create table if not exists test6 (
  	test6_id int,
      val int,
      col int,
  	PRIMARY KEY(test6_id)
  ) engine=InnoDB;
  select connection_id();
  insert into test6 values(1,2,3);
  insert into test6 values(2,3,4);
  commit;
  insert into test6 values(3,3,7);
  rollback;
  insert into test6 values(4,5,6);
  commit;
  ```

  ```mysql
  #conn4.sql
  	
  #problem 1
  set autocommit = 0;
  drop procedure if exists problem1;
  delimiter //
  create procedure problem1()
  begin
  	select connection_id();
  	select * from test1 where test1_id = '1';
  end
  //
  delimiter ; 
  call problem1;
  commit;
  
  
  #problem 2
  set autocommit = 0;
  drop procedure if exists problem2;
  delimiter //
  create procedure problem2()
  begin
  	select connection_id();
  	select * from test2 where val = '3' lock in share mode;
  end
  //
  delimiter ; 
  call problem2;
  commit;
  
  #problem 3
  set autocommit = 0;
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
  commit;
  
  #problem 4 
  set autocommit = 0;
  drop procedure if exists problem4;
  delimiter //
  create procedure problem4()
  begin
  	select connection_id();
      select * from test4 where test4_id = 3 lock in share mode;
  end
  //
  delimiter ; 
  call problem4;
  commit;
  
  #problem 5
  set autocommit = 0;
  drop procedure if exists problem5;
  delimiter //
  create procedure problem5()
  begin
  	select connection_id();
      select * from test5 where col = 7 lock in share mode;
  end
  //
  delimiter ; 
  call problem5;
  commit;
  
  #problem 6
  set autocommit = 0;
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
  commit;
  ```

  