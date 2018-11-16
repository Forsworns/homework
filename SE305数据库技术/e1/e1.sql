/*
不要用中文目录
信息不全导致用外码出错

limit改到50000rows

进命令行
show database
use ****
调试
*/
-- 建表
drop table cast_members;
drop table directors;
drop table people;
drop table ratings;
drop table movies;

create table hw1_imdb.movies(
	id char(9),
    mname char(200),
    myear int,
    mtime int,--上映时间之后改一下改成mtime
    primary key(id)
);

create table hw1_imdb.ratings(
	id char(9),
    mscore double,
    mtimes int,
    primary key (id)
);

create table hw1_imdb.people(
	pid char(9),
    pname char(50),
    pbirth int,
    pdeath int,
    primary key (pid)
);

create table hw1_imdb.cast_members(
	id char(9),
    pid char(9),
    primary key (id,pid)
);

create table hw1_imdb.directors(
	id char(9),
    pid char(9),
    primary key (id,pid)
);
-- 然后导入数据

-- e1
insert into movies value(
	'tt1234567','uncle drew',2018,44
);

insert into ratings value(
	'tt1234567',9.8,10000
);

insert into people value(
	'nm1234567','Kyrie Irving',1726,1986
);

insert into cast_members value(
	'tt1234567','nm1234567'
);

insert into directors value(
	'tt1234567','nm1234567'
);

-- e2
select pname
from people
where pname like 'John _%';

-- e3
select mname,myear
from movies
order by myear asc
limit 0,10;

-- e4
select avg(mtime)
from movies
where myear=1914;

-- e5
select mname
from movies
where id=(
	select id
	from ratings
	order by mscore DESC
	limit 1
);
-- 高分可能不止一个
select mname
from movies
where id in (
	select id
	from ratings
	where mscore=(
		select max(mscore)
        from ratings
    )
);

-- e6
select mname
from movies
	where id in (
	select id
	from cast_members
    where pid in (
		select pid
        from people
        where pname='L. Frank Baum'
	)
);

-- e7
select avg(mtime)
from movies
where id in(
	select id
    from ratings
    where mscore>9.0
);

-- e8
select count(pid)
from cast_members
where id in (
	select id
    from movies
    where mtime=(
		select max(mtime)
        from movies
    )
);

-- e9
insert into people(pid)
	select distinct pid
    from directors
    where pid not in(
		select pid
		from people
    );
-- 先将缺失pid的补满成null

select pname
from people
where pid=(
	select pid
	from directors
	group by pid
	order by count(id) desc
	limit 1
)
-- 多人版
select pname
from people
where pid in (
	select r.pid
	from (
		select pid,count(id) as countid
		from directors
		group by pid
	) as r
	where r.countid=(
		select max(t.countid)
		from (
			select count(id) as countid
			from directors
			group by pid
		) as t
	)
)

-- e10
select pname
from people
where pid =(
	select t.pid
	from (
		select directors.pid,count(directors.id)
		from cast_members,directors
		where cast_members.id=directors.id and cast_members.pid=directors.pid
		group by directors.pid
		order by count(directors.id) desc
		limit 1
	) as t
);
-- 多人版
select pname
from people
where pid =(
	select t.pid
	from (
		select directors.pid,count(directors.id) as directorsid
		from cast_members,directors
		where cast_members.id=directors.id and cast_members.pid=directors.pid
		group by directors.pid
	) as t
    where t.directorsid =(
		select max(r.directorsid)
		from (
			select count(directors.id) as directorsid
			from cast_members,directors
            where cast_members.id=directors.id and cast_members.pid=directors.pid
            group by directors.pid
		) as r
    )
);

-- e11
insert into people(pid)
	select distinct pid
    from cast_members
    where pid not in(
		select pid
		from people
    );
-- 先将缺失pid的补满成null

select pname
from people
where pid =(
	select t.pid
    from(
		select max(movies.myear)-min(movies.myear) as pro_life,cast_members.pid as pid
		from movies,cast_members
		where movies.id=cast_members.id
		group by cast_members.pid
		order by pro_life desc
		limit 1
    ) as t
);
-- 多人版
select pname
from people
where pid =(
	select t.pid
	from(
		select max(movies.myear)-min(movies.myear) as pro_life,cast_members.pid as pid
		from movies,cast_members
		where movies.id=cast_members.id
		group by cast_members.pid
    ) as t
	where t.pro_life = (
		select max(r.pro_life)
        from (
			select max(movies.myear)-min(movies.myear) as pro_life,cast_members.pid as pid
			from movies,cast_members
			where movies.id=cast_members.id
			group by cast_members.pid
        ) as r
    )
);
-- e12
alter table people
	add age int;
	
SET SQL_SAFE_UPDATES = 0; -- 屏蔽安全模式(非主键where无法更新)
update people
set age=(
	case people.pdeath
		when null
		then 
		2018-pbirth
		else
		pdeath-pbirth
		end
);

-- select age,pbirth,pdeath
-- from people;

-- e13
alter table people
add column ptimes int;

update people 
set ptimes = (
	select t.counts from(
		SELECT pid,count(id) counts
		from cast_members
		group by pid) as t
	where people.pid = t.pid
);

-- e14
select pname
from people
where pid in(
	select pid
	from directors
	where id in(
		select id
		from ratings
		where
        id in(
				select id
				from cast_members
				where pid=(
					select pid
					from people
					where ptimes=(
						select max(ptimes)
						from people
					)
				)
			)
        and
        mscore=(
			select max(t.mscore)
			from(
				select mscore
				from ratings
				where id in(
					select id
					from cast_members
					where pid=(
						select pid
						from people
						where ptimes=(
							select max(ptimes)
							from people
						)
					)
				)
			) as t
		)
	)
);

-- e15
SET SQL_SAFE_UPDATES = 0;

update ratings
set mscore=mscore+0.5;

alter table ratings
drop mtimes;

-- e16
create index index_pname on people(pname);

select pname -- 由外向内
from people
where pid in (
	select pid
    from people
    where age=(
		select max(r.age)
		from (
		select age
		from people
		where pid in(
			select pid
			from people
			where 
			pid in(
				select t.pid
				from (
					select pid
					from directors
					where id in(
						select id
						from movies
						where myear>1905 and myear<1949
					)
				) as t
			)
		)
		) as r
    )
);

drop index index_pname on people;

-- e17
delete from cast_members
where id in (
	select id
	from ratings
	where mscore<5.0
)；

-- e18
create view cast_view as
select avg(movies.mtime) cast_time,people.pid
from movies,people
where movies.id in (
	select id
    from cast_members
    where pid=people.pid
);

/*select pid
    from cast_view
    where cast_time = (
		select min(cast_time)
        from cast_view
    );*/
    
delete from cast_members
where pid in (
	select pid
    from cast_view
    where cast_time = (
		select min(cast_time)
        from cast_view
    )
);

drop view cast_view;