#include<iostream>
#include<string>
#include"sqlite3.h"
#include"httplib.h"
bool studentselect(sqlite3* db,const std::string& studentID,const std::string& courseid)
{
    std::string tablename="Schedule"+studentID;
    std::string sql1="select * from "+tablename+" order by starting_time asc;";


    const char* sql=
        "select * " 
        "from Course " 
        "where course_id=?";

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
        std::cerr<<"Failed to bind studentID"<<sqlite3_errmsg(db)<<std::endl;;
        return false;
    }

    rc=sqlite3_step(stmt);

    if(rc==SQLITE_ROW)
    {
        //const unsigned char* dbcourse_name=sqlite3_column_text(stmt,1);
        //const unsigned char* dbcredits=sqlite3_column_text(stmt,2);
        //const unsigned char* dbdepartment=sqlite3_column_text(stmt,3);
        const unsigned char* dbstarting_time=sqlite3_column_text(stmt,4);
        const unsigned char* dbending_time=sqlite3_column_text(stmt,5);
        const unsigned char* dbweek_day=sqlite3_column_text(stmt,6);
        //const unsigned char* dbclass_location=sqlite3_column_text(stmt,7);
        std::string starting_time=dbstarting_time?reinterpret_cast<const char*>(dbstarting_time):"";
        std::string ending_time=dbending_time?reinterpret_cast<const char*>(dbending_time):"";
        std::string week_day=dbweek_day?reinterpret_cast<const char*>(dbweek_day):"";

        sqlite3_stmt* stmt_schdule=nullptr;
        rc=sqlite3_prepare_v2(db,sql1.c_str(),-1,&stmt_schdule,nullptr);
        if(rc!=SQLITE_OK)
        {
            std::cerr<<"Failed to compile schduleSQL"<<sqlite3_errmsg(db)<<std::endl;
            sqlite3_finalize(stmt_schdule);
            sqlite3_finalize(stmt);
            return false;
        }     
        
        
        std::string lastcourseendingtime="";
        while((rc=sqlite3_step(stmt_schdule))==SQLITE_ROW)
        {
            const unsigned char* coursestartingtime=sqlite3_column_text(stmt_schdule,1);
            const unsigned char* courseendingtime=sqlite3_column_text(stmt_schdule,2);
            const unsigned char* courseweekday=sqlite3_column_text(stmt_schdule,3);
            std::string schedulestartingtime=coursestartingtime?reinterpret_cast<const char*>(coursestartingtime):"";
            std::string schedulesendingtime=courseendingtime?reinterpret_cast<const char*>(courseendingtime):"";
            std::string schedulesweekday=courseweekday?reinterpret_cast<const char*>(courseweekday):"";
            if(schedulesweekday==week_day)
            {
                if(starting_time<schedulestartingtime)
                {
                    if(ending_time>=schedulestartingtime)
                    {
                        sqlite3_finalize(stmt);
                        sqlite3_finalize(stmt_schdule);
                        return false;
                    }
                    else
                    {
                        if(starting_time<=lastcourseendingtime)
                        {
                            sqlite3_finalize(stmt);
                            sqlite3_finalize(stmt_schdule);
                            return false;
                        }
                        else
                        {
                                const std::string sqlinsert=std::string("INSERT INTO Schedule")+studentID+std::string(" (course_id, starting_time, ending_time, week_day, status) VALUES (?, ?, ?, ?, 'pending');");
                                sqlite3_stmt* stmt1=nullptr;
                                int rc3=sqlite3_prepare_v2(db,sqlinsert.c_str(),-1,&stmt1,nullptr);
                                if(rc3!=SQLITE_OK)
                                {
                                    std::cerr<<"Failed to compile addcourseSQL"<<sqlite3_errmsg(db)<<std::endl;
                                    return false;
                                }
                                rc3=sqlite3_bind_text(stmt1,1,courseid.c_str(),-1,SQLITE_TRANSIENT);
                                if(rc3!=SQLITE_OK)
                                {
                                    std::cerr<<"Failed to bind addcourseID"<<sqlite3_errmsg(db)<<std::endl;
                                    return false;
                                }
                                rc3=sqlite3_bind_text(stmt1,2,starting_time.c_str(),-1,SQLITE_TRANSIENT);
                                if(rc3!=SQLITE_OK)
                                {
                                    std::cerr<<"Failed to bind addcourseID"<<sqlite3_errmsg(db)<<std::endl;
                                    return false;
                                }
                                rc3=sqlite3_bind_text(stmt1,3,ending_time.c_str(),-1,SQLITE_TRANSIENT);
                                if(rc3!=SQLITE_OK)
                                {
                                    std::cerr<<"Failed to bind addcourseID"<<sqlite3_errmsg(db)<<std::endl;
                                    return false;
                                }
                                rc3=sqlite3_bind_text(stmt1,4,week_day.c_str(),-1,SQLITE_TRANSIENT);
                                if(rc3!=SQLITE_OK)
                                {
                                    std::cerr<<"Failed to bind addcourseID"<<sqlite3_errmsg(db)<<std::endl;
                                    return false;
                                }
                                
                                rc3=sqlite3_step(stmt1);
                                sqlite3_finalize(stmt1);
                                sqlite3_finalize(stmt);
                                sqlite3_finalize(stmt_schdule);
                                return true;
    
                        }
                    }
                }
                lastcourseendingtime=schedulesendingtime;
            }


            //
        } 

        if(lastcourseendingtime!="")
        {
            if(starting_time<=lastcourseendingtime)
                        {
                            sqlite3_finalize(stmt);
                            sqlite3_finalize(stmt_schdule);
                            return false;
                        }
                        else
                        {
                                const std::string sqlinsert=std::string("INSERT INTO Schedule")+studentID+std::string(" (course_id, starting_time, ending_time, week_day, status) VALUES (?, ?, ?, ?, 'pending');");
                                sqlite3_stmt* stmt1=nullptr;
                                int rc3=sqlite3_prepare_v2(db,sqlinsert.c_str(),-1,&stmt1,nullptr);
                                if(rc3!=SQLITE_OK)
                                {
                                    std::cerr<<"Failed to compile addcourseSQL"<<sqlite3_errmsg(db)<<std::endl;
                                    return false;
                                }
                                rc3=sqlite3_bind_text(stmt1,1,courseid.c_str(),-1,SQLITE_TRANSIENT);
                                if(rc3!=SQLITE_OK)
                                {
                                    std::cerr<<"Failed to bind addcourseID"<<sqlite3_errmsg(db)<<std::endl;
                                    return false;
                                }
                                rc3=sqlite3_bind_text(stmt1,2,starting_time.c_str(),-1,SQLITE_TRANSIENT);
                                if(rc3!=SQLITE_OK)
                                {
                                    std::cerr<<"Failed to bind addcourseID"<<sqlite3_errmsg(db)<<std::endl;
                                    return false;
                                }
                                rc3=sqlite3_bind_text(stmt1,3,ending_time.c_str(),-1,SQLITE_TRANSIENT);
                                if(rc3!=SQLITE_OK)
                                {
                                    std::cerr<<"Failed to bind addcourseID"<<sqlite3_errmsg(db)<<std::endl;
                                    return false;
                                }
                                rc3=sqlite3_bind_text(stmt1,4,week_day.c_str(),-1,SQLITE_TRANSIENT);
                                if(rc3!=SQLITE_OK)
                                {
                                    std::cerr<<"Failed to bind addcourseID"<<sqlite3_errmsg(db)<<std::endl;
                                    return false;
                                }
                                
                                rc3=sqlite3_step(stmt1);
                                sqlite3_finalize(stmt1);
                                sqlite3_finalize(stmt_schdule);
                                sqlite3_finalize(stmt);
                                return true;
    
                        }
        }   
        else
        {
             const std::string sqlinsert=std::string("INSERT INTO Schedule")+studentID+std::string(" (course_id, starting_time, ending_time, week_day, status) VALUES (?, ?, ?, ?, 'pending');");
                                sqlite3_stmt* stmt1=nullptr;
                                int rc3=sqlite3_prepare_v2(db,sqlinsert.c_str(),-1,&stmt1,nullptr);
                                if(rc3!=SQLITE_OK)
                                {
                                    std::cerr<<"Failed to compile addcourseSQL"<<sqlite3_errmsg(db)<<std::endl;
                                    return false;
                                }
                                rc3=sqlite3_bind_text(stmt1,1,courseid.c_str(),-1,SQLITE_TRANSIENT);
                                if(rc3!=SQLITE_OK)
                                {
                                    std::cerr<<"Failed to bind addcourseID"<<sqlite3_errmsg(db)<<std::endl;
                                    return false;
                                }
                                rc3=sqlite3_bind_text(stmt1,2,starting_time.c_str(),-1,SQLITE_TRANSIENT);
                                if(rc3!=SQLITE_OK)
                                {
                                    std::cerr<<"Failed to bind addcourseID"<<sqlite3_errmsg(db)<<std::endl;
                                    return false;
                                }
                                rc3=sqlite3_bind_text(stmt1,3,ending_time.c_str(),-1,SQLITE_TRANSIENT);
                                if(rc3!=SQLITE_OK)
                                {
                                    std::cerr<<"Failed to bind addcourseID"<<sqlite3_errmsg(db)<<std::endl;
                                    return false;
                                }
                                rc3=sqlite3_bind_text(stmt1,4,week_day.c_str(),-1,SQLITE_TRANSIENT);
                                if(rc3!=SQLITE_OK)
                                {
                                    std::cerr<<"Failed to bind addcourseID"<<sqlite3_errmsg(db)<<std::endl;
                                    return false;
                                }
                                
                                rc3=sqlite3_step(stmt1);
                                sqlite3_finalize(stmt1);
                                sqlite3_finalize(stmt_schdule);
                                sqlite3_finalize(stmt);
                                return true;
    
        }
    

    }
    else
    {
        std::cerr<<"Something went wrong!"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    
}

