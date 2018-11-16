CREATE TABLE `sys`.`movies` (
  `id` VARCHAR(9) NOT NULL,
  `name` VARCHAR(200) NOT NULL,
  `year` INT(11) NOT NULL,
  `time` INT(11) NULL,
  PRIMARY KEY (`id`));

CREATE TABLE `sys` . `ratings`(
  `movie_id` VARCHAR(9) NOT NULL,
  `value` DOUBLE NOT NULL,
  `times` INT(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`movie_id`));
  
CREATE TABLE `sys`.`people`(
  `id` VARCHAR(9) NOT NULL,
  `name` VARCHAR(50) NOT NULL,
  `birth` INT(11) NULL,
  `death` INT(11) NULL,
  PRIMARY KEY (`id`));
  
CREATE TABLE `sys`.`cast_members` (
  `movie_id` VARCHAR(9) NOT NULL,
  `people_id` VARCHAR(9) NOT NULL,
  PRIMARY KEY (`movie_id`, `people_id`));

CREATE TABLE `sys`.`directors` (
  `movie_id` VARCHAR(9) NOT NULL,
  `people_id` VARCHAR(9) NOT NULL,
  PRIMARY KEY (`movie_id`, `people_id`));



#problem 1
INSERT INTO `sys`.`movies` (`id`, `name`, `year`, `time`) VALUES ('tt0005011', 'test', '2018', '120');
INSERT INTO `sys`.`people` (`id`, `name`, `birth`, `death`) VALUES ('nm0017766', 'test', '1994', NULL);
INSERT INTO `sys`.`ratings` (`movie_id`, `value`, `times`) VALUES ('tt0005117', '5', '1');
INSERT INTO `sys`.`cast_members` (`movie_id`, `people_id`) VALUES ('tt0005117', 'nm0017766');
INSERT INTO `sys`.`directors` (`movie_id`, `people_id`) VALUES ('tt0005117', 'nm0017794');

#problem 2
SELECT `name` FROM `sys`.`people` WHERE (`name` LIKE 'John%');

#problem 3
SELECT `name`, `year` FROM `sys`.`movies` ORDER BY `year` LIMIT 0,10;

#problem 
SELECT AVG(`time`) FROM `sys`.`movies` WHERE ( `year` = '1914' ); 

#problem 5
SELECT `name` FROM `sys`.`movies` WHERE `id` =
(select `movie_id`
	from(
		select `movie_id`,max(`value`)
		from(
				SELECT `movie_id`,`value` 
				FROM `sys`.`ratings` 
				group by `movie_id` order by `value` DESC 
			)t1
	)t2
);

#problem 6
SELECT `name` FROM `sys`.`movies` WHERE `id` IN
(SELECT `movie_id` FROM `sys`.`cast_members` WHERE (`people_id` IN 
(SELECT `id` FROM `sys`.`people` WHERE `name` LIKE 'L. Frank Baum' )));

#problem 7
SELECT AVG(`time`) FROM `sys`.`movies` WHERE `id` IN 
(SELECT `movie_id` FROM `sys`.`ratings` WHERE `value` > 9.0 );

#problem 8
SELECT COUNT(*) FROM `sys`.`cast_members` WHERE (`movie_id` = (
	select `id` from(
		select `id`, max(`time`)
		from(
			SELECT `id`,`time` FROM `sys`.`movies` group by `id` order by `time` DESC
		)t1
	)t2
)); 

#problem 9
select `name` from `sys`.`people` where (`id` = (
select `people_id` from
(select `people_id`, count(*) as `number` from `sys`.`directors` 
group by `people_id` order by `number` desc limit 0,1)t));

#problem 10
select `name` from `sys`.`people` where `id` IN (
	select `people_id` from(
		select `people_id`, count(*) as `number` from(
			select `sys`.`directors`.people_id
			from `sys`.`directors`, `sys`.`cast_members`
			where `sys`.`directors`.movie_id = `sys`.`cast_members`.movie_id
		)t1
		group by `people_id` order by `number` desc
	)t2
	where `number` = (
		select max(`number`) from(
			select `people_id`, count(*) as `number` from(
				select `sys`.`directors`.people_id
				from `sys`.`directors`, `sys`.`cast_members`
				where `sys`.`directors`.movie_id = `sys`.`cast_members`.movie_id
			)t3
			group by `people_id` order by `number` desc
		)t4
	)
);

#problem 11
select `name` from `sys`.`people` where `id` = (
	select `people_id` from(
		select `people_id`, max(`year`)-min(`year`) as `career`
		from `sys`.`cast_members`, `sys`.`movies` 
		where `cast_members`.`movie_id`=`movies`.`id`
		group by `cast_members`.`people_id` order by `career` desc limit 0,1
	)t
);

#problem12
ALTER TABLE `sys`.`people` 
ADD COLUMN `lifetime` INT(11) NULL AFTER `death`;

update `sys`.`people`
set `lifetime` = `death` - `birth`
where `death` is not null OR `birth` is not null;

#problem13
ALTER TABLE `sys`.`people` 
ADD COLUMN `cast_times` INT(11) NULL AFTER `lifetime`;


update `sys`.`people`,
(
	select `people_id`, count(*) as `counts` 
	from `sys`.`people`, `sys`.`cast_members`
	where `people`.`id` = `cast_members`.`people_id`
	group by `people_id`
)t1
set `cast_times` = t1.`counts`
where `people`.`id`=t1.`people_id`;

#problem 14
select *
from
(
	select t4.`movie_id`, max(`value`), `directors`.people_id
	from(
		select t3.`movie_id`,`value`
		from(
			select t2.`people_id`,`movie_id`
			from(
				select `people_id`, max(`counts`)
				from(
					select `people_id`, count(*)as counts
					from `sys`.`cast_members`
					group by `people_id` order by `counts` desc
				)t1
			)t2,
			`sys`.`cast_members`
			where t2.`people_id`=`cast_members`.`people_id`
		)t3,
		`sys`.`ratings`
		where t3.`movie_id`=`ratings`.`movie_id`
		group by `movie_id` order by `value` desc
	)t4,
	`sys`.`directors`
	where t4.`movie_id`=`directors`.movie_id
	order by `value` desc
)t5,
`sys`.`people`
where t5.people_id=id;

#problem 15
update `sys`.`ratings`,(
	select *
    from `sys`.`movies`
    where `year`='1919'
)t1
set `value`=`value`+0.5
where `t1`.`id`=`ratings`.`movie_id`;

delete `times` from `sys`.`ratings`;

#problem16
create index pIndex on `sys`.`people`(`name`); 
	select `people`.`name`
	from `sys`.`people`
	where `lifetime` in (
	select max(`lifetime`)
	from `sys`.`people`, `sys`.movies, `sys`.directors
	where people.id=directors.people_id AND directors.movie_id=movies.id AND `movies`.`year` between '1905'and'1949'
	order by `lifetime`
);
drop index pIndex on `sys`.`people`;

#problem 17
delete
from cast_members
where movie_id in (
	select movie_id
	from ratings
	where ratings.`value`<'5'
);

delete
from directors
where movie_id in (
	select movie_id
	from ratings
    where ratings.value<'5'
);


#problem 18
create view avgtime
as
	select `people_id`, avg(`time`)as `avgt`
    from `sys`.`cast_members`, `sys`.`movies`
    where `cast_members`.`movie_id`=`movies`.`id`
    group by `people_id` order by `avgt` desc;
    
delete
from `sys`.`cast_members` 
where `people_id`in(
	select `people_id`
	from `avgtime`
	where `avgt` = (
		select max(`avgt`)
		from `avgtime`
	)
);
drop view avgtime;
