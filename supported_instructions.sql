create table student (
	sno char(8),
	sname char(16) unique,
	sage int,
	sgender char (1),
	primary key ( sno )
);
create index stunameidx on student ( sname );
drop index stunameidx;
insert into student values ('12345678','wy',22,'M');
select * from student where sage > 20 and sgender <> 'F';
delete from student where sno = '88888888';
drop table student;
quit