pragma foreign_keys=ON;
create table if not exists Student
(
    student_id text primary key,
    password text not null,
    name text,
    major text,
    grade integer,
    contact_info text
);

create table if not exists Admin
(
    admin_id text primary key,
    password text not null,
    name text
);

create table if not exists Course
(
    course_id text primary key,
    course_name text not null,
    credits text,
    department text,
    starting_time text,
    ending_time text,
    week_day text,
    class_location text,
    student_number integer
);

