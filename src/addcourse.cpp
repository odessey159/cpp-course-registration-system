#include<iostream>
#include<string>
#include"sqlite3.h"
#include"httplib.h"
bool adminadd(sqlite3* db,const std::string& courseid,const std::string& name,const std::string& credits,const std::string& department,const std::string& starting_time,const std::string& ending_time,const std::string& week_day,const std::string& location)
{
    const char* sql=
        "insert into Course" 
        " (course_id, course_name, credits, department, starting_time, ending_time, week_day, class_location, student_number)" 
        " values(?,?,?,?,?,?,?,?,0)";

    sqlite3_stmt* stmt=nullptr;

    int rc=sqlite3_prepare_v2(db,sql,-1,&stmt,nullptr);
    if(rc!=SQLITE_OK)
    {
        std::cerr<<"Failed to compile SQL"<<sqlite3_errmsg(db)<<std::endl;
        return false;
    }
    rc=sqlite3_bind_text(stmt,1,courseid.c_str(),-1,SQLITE_TRANSIENT);
    if(rc!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind courseID"<<sqlite3_errmsg(db)<<std::endl;;
        sqlite3_finalize(stmt);
        return false;
    }
    rc=sqlite3_bind_text(stmt,2,name.c_str(),-1,SQLITE_TRANSIENT);
    if(rc!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind courseID"<<sqlite3_errmsg(db)<<std::endl;;
        sqlite3_finalize(stmt);
        return false;
    }
    rc=sqlite3_bind_text(stmt,3,credits.c_str(),-1,SQLITE_TRANSIENT);
    if(rc!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind courseID"<<sqlite3_errmsg(db)<<std::endl;;
        sqlite3_finalize(stmt);
        return false;
    }
    
    rc=sqlite3_bind_text(stmt,4,department.c_str(),-1,SQLITE_TRANSIENT);
    if(rc!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind courseID"<<sqlite3_errmsg(db)<<std::endl;;
        sqlite3_finalize(stmt);
        return false;
    }
    rc=sqlite3_bind_text(stmt,5,starting_time.c_str(),-1,SQLITE_TRANSIENT);
    if(rc!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind courseID"<<sqlite3_errmsg(db)<<std::endl;;
        sqlite3_finalize(stmt);
        return false;
    }
    rc=sqlite3_bind_text(stmt,6,ending_time.c_str(),-1,SQLITE_TRANSIENT);
    if(rc!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind courseID"<<sqlite3_errmsg(db)<<std::endl;;
        sqlite3_finalize(stmt);
        return false;
    }
    rc=sqlite3_bind_text(stmt,7,week_day.c_str(),-1,SQLITE_TRANSIENT);
    if(rc!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind courseID"<<sqlite3_errmsg(db)<<std::endl;;
        sqlite3_finalize(stmt);
        return false;
    }
    rc=sqlite3_bind_text(stmt,8,location.c_str(),-1,SQLITE_TRANSIENT);
    if(rc!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind courseID"<<sqlite3_errmsg(db)<<std::endl;;
        sqlite3_finalize(stmt);
        return false;
    }

    rc=sqlite3_step(stmt);

    if(rc==SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return true;
    }
    else
    {
        std::cerr<<"Something went wrong!"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    
}

bool admindelete(sqlite3* db,const std::string& courseid)
{
    std::string sql="delete from Course where course_id=?;";


    sqlite3_stmt* stmt=nullptr;

    int rc=sqlite3_prepare_v2(db,sql.c_str(),-1,&stmt,nullptr);
    if(rc!=SQLITE_OK)
    {
        std::cerr<<"Failed to compile SQL"<<sqlite3_errmsg(db)<<std::endl;
        return false;
    }
    rc=sqlite3_bind_text(stmt,1,courseid.c_str(),-1,SQLITE_TRANSIENT);
    if(rc!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind courseID"<<sqlite3_errmsg(db)<<std::endl;;
        return false;
    }

    rc=sqlite3_step(stmt);

    if(rc==SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return true;
    }
    else
    {
        std::cerr<<"Something went wrong!"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    
}
