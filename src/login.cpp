#include<iostream>
#include<string>
#include"sqlite3.h"
#include"httplib.h"
bool studentlogin(sqlite3* db,const std::string& studentID,const std::string& password,std::string& outname)
{
    const char* sql=
        "select password, name " 
        "from Student " 
        "where student_id=?";

    sqlite3_stmt* stmt=nullptr;

    int rc=sqlite3_prepare_v2(db,sql,-1,&stmt,nullptr);
    if(rc!=SQLITE_OK)
    {
        std::cerr<<"Failed to compile SQL"<<sqlite3_errmsg(db)<<std::endl;
        return false;
    }
    rc=sqlite3_bind_text(stmt,1,studentID.c_str(),-1,SQLITE_TRANSIENT);
    if(rc!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind studentID"<<sqlite3_errmsg(db)<<std::endl;
        return false;
    }

    rc=sqlite3_step(stmt);

    if(rc==SQLITE_ROW)
    {
        const unsigned char* dbPassword=sqlite3_column_text(stmt,0);
        const unsigned char* dbName=sqlite3_column_text(stmt,1);
        std::string realPassword=dbPassword?reinterpret_cast<const char*>(dbPassword):"";
        std::string name=dbName?reinterpret_cast<const char*>(dbName):"";
        if(realPassword==password)
        {
            outname=name;
            sqlite3_finalize(stmt);
            return true;
        }
        else
        {
            sqlite3_finalize(stmt);
            return false;
        }
    }
    else if(rc==SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return false;
    }
    else
    {
        std::cerr<<"Something went wrong!"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
}

bool adminlogin(sqlite3* db,const std::string& studentID,const std::string& password,std::string& outname)
{
    const char* sql=
        "select password, name " 
        "from Admin " 
        "where admin_id=?";

    sqlite3_stmt* stmt=nullptr;

    int rc=sqlite3_prepare_v2(db,sql,-1,&stmt,nullptr);
    if(rc!=SQLITE_OK)
    {
        std::cerr<<"Failed to compile SQL"<<sqlite3_errmsg(db)<<std::endl;
        return false;
    }
    rc=sqlite3_bind_text(stmt,1,studentID.c_str(),-1,SQLITE_TRANSIENT);
    if(rc!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind studentID"<<sqlite3_errmsg(db)<<std::endl;
        return false;
    }

    rc=sqlite3_step(stmt);

    if(rc==SQLITE_ROW)
    {
        const unsigned char* dbPassword=sqlite3_column_text(stmt,0);
        const unsigned char* dbName=sqlite3_column_text(stmt,1);
        std::string realPassword=dbPassword?reinterpret_cast<const char*>(dbPassword):"";
        std::string name=dbName?reinterpret_cast<const char*>(dbName):"";
        if(realPassword==password)
        {
            outname=name;
            sqlite3_finalize(stmt);
            return true;
        }
        else
        {
            sqlite3_finalize(stmt);
            return false;
        }
    }
    else if(rc==SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return false;
    }
    else
    {
        std::cerr<<"Something went wrong!"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
}

bool studentregister(sqlite3* db,const std::string& studentID,const std::string& password,std::string& outname,std::string& major,std::string& grade)
{
    std::string tablename="Schedule"+studentID;
    const char* sql=
        "SELECT count(*) "
        "from Student "
        "where student_id=?;";

    const char* sqlinsert=
        "INSERT INTO Student (student_id, password, name, major, grade) "
        "VALUES (?, ?, ?, ?, ?);";

    std::string sqlnewtable="create table if not exists "+tablename+" (course_id text primary key, starting_time text, ending_time text, week_day text, status text)";

    sqlite3_stmt* stmt=nullptr;

    int rc=sqlite3_prepare_v2(db,sql,-1,&stmt,nullptr);
    if(rc!=SQLITE_OK)
    {
        std::cerr<<"Failed to compile SQL"<<sqlite3_errmsg(db)<<std::endl;
        return false;
    }
    rc=sqlite3_bind_text(stmt,1,studentID.c_str(),-1,SQLITE_TRANSIENT);
    if(rc!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind studentID"<<sqlite3_errmsg(db)<<std::endl;
        return false;
    }

    rc=sqlite3_step(stmt);

    if(rc==SQLITE_ROW)
    {
        const int inserted=sqlite3_column_int(stmt,0);
        if(inserted>0)
        {
            sqlite3_finalize(stmt);
            return false;
        }
    }
    else
    {
        std::cerr<<"Something went wrong!"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    int rc1=sqlite3_prepare_v2(db,sqlinsert,-1,&stmt,nullptr);
    if(rc1!=SQLITE_OK)
    {
        std::cerr<<"Failed to compile SQLinsert"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    rc1=sqlite3_bind_text(stmt,1,studentID.c_str(),-1,SQLITE_TRANSIENT);
    if(rc1!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    rc1=sqlite3_bind_text(stmt,2,password.c_str(),-1,SQLITE_TRANSIENT);
    if(rc1!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    rc1=sqlite3_bind_text(stmt,3,outname.c_str(),-1,SQLITE_TRANSIENT);
    if(rc1!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    rc1=sqlite3_bind_text(stmt,4,major.c_str(),-1,SQLITE_TRANSIENT);
    if(rc1!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    rc1=sqlite3_bind_text(stmt,5,grade.c_str(),-1,SQLITE_TRANSIENT);
    if(rc1!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    rc1=sqlite3_step(stmt);

    int rc2=sqlite3_prepare_v2(db,sqlnewtable.c_str(),-1,&stmt,nullptr);
    if(rc2!=SQLITE_OK)
    {
        std::cerr<<"Failed to compile SQLinsert"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    rc2=sqlite3_step(stmt);
    
    sqlite3_finalize(stmt);

    return true;
}

bool studentmodify(sqlite3* db,const std::string& studentID,const std::string& password,std::string& outname,std::string& major,std::string& grade)
{
    const char* sql=
        "update Student " 
        "set password=?, name=?, major=?, grade=? "
        "where student_id=?";

    sqlite3_stmt* stmt=nullptr;

    int rc1=sqlite3_prepare_v2(db,sql,-1,&stmt,nullptr);
    if(rc1!=SQLITE_OK)
    {
        std::cerr<<"Failed to compile SQL"<<sqlite3_errmsg(db)<<std::endl;
        return false;
    }
    rc1=sqlite3_bind_text(stmt,5,studentID.c_str(),-1,SQLITE_TRANSIENT);
    if(rc1!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    rc1=sqlite3_bind_text(stmt,1,password.c_str(),-1,SQLITE_TRANSIENT);
    if(rc1!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    rc1=sqlite3_bind_text(stmt,2,outname.c_str(),-1,SQLITE_TRANSIENT);
    if(rc1!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    rc1=sqlite3_bind_text(stmt,3,major.c_str(),-1,SQLITE_TRANSIENT);
    if(rc1!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    rc1=sqlite3_bind_text(stmt,4,grade.c_str(),-1,SQLITE_TRANSIENT);
    if(rc1!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    rc1=sqlite3_step(stmt);
    
    sqlite3_finalize(stmt);
    return true;
}

bool adminmodify(sqlite3* db,const std::string& studentID,const std::string& password,std::string& outname)
{
    const char* sql=
        "update Admin " 
        "set password=?, name=? "
        "where admin_id=?";

    sqlite3_stmt* stmt=nullptr;

    int rc1=sqlite3_prepare_v2(db,sql,-1,&stmt,nullptr);
    if(rc1!=SQLITE_OK)
    {
        std::cerr<<"Failed to compile SQL"<<sqlite3_errmsg(db)<<std::endl;
        return false;
    }

    rc1=sqlite3_bind_text(stmt,3,studentID.c_str(),-1,SQLITE_TRANSIENT);
    if(rc1!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    rc1=sqlite3_bind_text(stmt,1,password.c_str(),-1,SQLITE_TRANSIENT);
    if(rc1!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    rc1=sqlite3_bind_text(stmt,2,outname.c_str(),-1,SQLITE_TRANSIENT);
    if(rc1!=SQLITE_OK)
    {
        std::cerr<<"Failed to bind"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    rc1=sqlite3_step(stmt);
    
    sqlite3_finalize(stmt);
    return true;
}







