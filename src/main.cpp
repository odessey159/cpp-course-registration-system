#include<iostream>
#include<string>
#include"sqlite3.h"
#include"httplib.h"
#include"json.hpp"
using json=nlohmann::json;

std::string current_user_id="";
std::string current_user_role="";

bool studentlogin(sqlite3* db,const std::string& studentID,const std::string& password,std::string& outname);
bool adminlogin(sqlite3* db,const std::string& studentID,const std::string& password,std::string& outname);

bool studentregister(sqlite3* db,const std::string& studentID,const std::string& password,std::string& outname,std::string& major,std::string& grade);

bool studentmodify(sqlite3* db,const std::string& studentID,const std::string& password,std::string& outname,std::string& major,std::string& grade);
bool adminmodify(sqlite3* db,const std::string& studentID,const std::string& password,std::string& outname);

bool studentselect(sqlite3* db,const std::string& studentID,const std::string& courseid);
bool studentdrop(sqlite3* db,const std::string& studentID,const std::string& courseid);


bool admindelete(sqlite3* db,const std::string& courseid);
bool adminadd(sqlite3* db,const std::string& courseid,const std::string& name,const std::string& credits,const std::string& department,const std::string& starting_time,const std::string& ending_time,const std::string& week_day,const std::string& location);

bool adminprogress(sqlite3* db,const std::string& studentID,const std::string& courseid,const std::string& status);

bool coursemodify(sqlite3* db,const std::string& courseID,const std::string& credits,std::string& coursetitle,std::string& department,std::string& location);

int main()
{
    sqlite3*db=nullptr;
    sqlite3_open("course_system.db",&db);
    sqlite3_exec(db,"PRAGMA foreign_keys=ON;",nullptr,nullptr,nullptr);

    httplib::Server svr;
    svr.set_default_headers({
        {"Access-Control-Allow-Origin","*"},
        {"Access-Control-Allow-Headers","Content-Type"},
        {"Access-Control-Allow-Methods","POST, OPTIONS"},
    });
    svr.Options("/login",[db](const httplib::Request& req,httplib::Response& res){
        res.status=200;
    });
    svr.Options("/register",[db](const httplib::Request& req,httplib::Response& res){
        res.status=200;
    });
    svr.Options("/modify",[db](const httplib::Request& req,httplib::Response& res){
        res.status=200;
    });
    svr.Options("/who",[db](const httplib::Request& req,httplib::Response& res){
        res.status=200;
    });
    svr.Options("/course_info",[db](const httplib::Request& req,httplib::Response& res){
        res.status=200;
    });
    svr.Options("/select",[db](const httplib::Request& req,httplib::Response& res){
        res.status=200;
    });
    svr.Options("/viewselectedcourses",[db](const httplib::Request& req,httplib::Response& res){
        res.status=200;
    });
    svr.Options("/drop",[db](const httplib::Request& req,httplib::Response& res){
        res.status=200;
    });
    svr.Options("/addcourse",[db](const httplib::Request& req,httplib::Response& res){
        res.status=200;
    });
    svr.Options("/deletecourse",[db](const httplib::Request& req,httplib::Response& res){
        res.status=200;
    });
    svr.Options("/progress",[db](const httplib::Request& req,httplib::Response& res){
        res.status=200;
    });
    svr.Options("/pendingrequest",[db](const httplib::Request& req,httplib::Response& res){
        res.status=200;
    });
    svr.Options("/coursestatue",[db](const httplib::Request& req,httplib::Response& res){
        res.status=200;
    });
    svr.Options("/coursemodify",[db](const httplib::Request& req,httplib::Response& res){
        res.status=200;
    });

    svr.Get("/who",[db](const httplib::Request& req,httplib::Response& res){
        json resp;
        resp["id"]=current_user_id;
        resp["role"]=current_user_role;
        res.set_content(resp.dump(),"application/json");
    });
    
    svr.Get("/course_info",[db](const httplib::Request& req,httplib::Response& res){
        std::string id=current_user_id;
        std::string role=current_user_role;

        const char* sql=
        "select * from Course";

        sqlite3_stmt* stmt=nullptr;
        int rc=sqlite3_prepare_v2(db,sql,-1,&stmt,nullptr);
        if(rc!=SQLITE_OK)
        {
            std::cerr<<"Failed to compile SQL"<<sqlite3_errmsg(db)<<std::endl;
            return;
        }
        

        json courses=json::array();

        while((rc=sqlite3_step(stmt))==SQLITE_ROW)
        {
            const unsigned char* courseid=sqlite3_column_text(stmt,0);
            const unsigned char* coursename=sqlite3_column_text(stmt,1);
            const unsigned char* coursecredits=sqlite3_column_text(stmt,2);
            const unsigned char* courseschool=sqlite3_column_text(stmt,3);
            const unsigned char* coursestartingtime=sqlite3_column_text(stmt,4);
            const unsigned char* courseendingtime=sqlite3_column_text(stmt,5);
            const unsigned char* courseweekday=sqlite3_column_text(stmt,6);
            const unsigned char* courselocation=sqlite3_column_text(stmt,7);
            json course;
            course["id"]=courseid?reinterpret_cast<const char*>(courseid):"";
            course["name"]=coursename?reinterpret_cast<const char*>(coursename):"";
            course["credits"]=coursecredits?reinterpret_cast<const char*>(coursecredits):"";
            course["school"]=courseschool?reinterpret_cast<const char*>(courseschool):"";
            course["startingtime"]=coursestartingtime?reinterpret_cast<const char*>(coursestartingtime):"";
            course["endingtime"]=courseendingtime?reinterpret_cast<const char*>(courseendingtime):"";
            course["weekday"]=courseweekday?reinterpret_cast<const char*>(courseweekday):"";
            course["location"]=courselocation?reinterpret_cast<const char*>(courselocation):"";
            courses.push_back(course);
        }
        sqlite3_finalize(stmt);
        json resp;
        resp["courses"]=courses;

        res.set_content(resp.dump(),"application/json");
    });

    svr.Post("/login",[db](const httplib::Request& req,httplib::Response& res){
        json recv=json::parse(req.body);
        std::string id=recv["id"];
        std::string pwd=recv["password"];
        std::string role=recv["role"];
        std::string name;
        bool logincheck=false;
        if(role=="student")
        {
            logincheck=studentlogin(db,id,pwd,name);
        }
        else
        {
            logincheck=adminlogin(db,id,pwd,name);
        }
        if(logincheck==true)
        {
            current_user_id=id;
            current_user_role=role;
        }
        json resp;
        resp["success"]=logincheck;

        res.set_content(resp.dump(),"application/json");
    });

    svr.Post("/modify",[db](const httplib::Request& req,httplib::Response& res){
        json recv=json::parse(req.body);
        std::string id=current_user_id;
        std::string pwd=recv["password"];
        std::string role=current_user_role;
        std::string name=recv["name"];
        bool logincheck=false;
        if(role=="student")
        {
            std::string major=recv["major"];
            std::string grade=recv["grade"];
            logincheck=studentmodify(db,id,pwd,name,major,grade);
        }
        else
        {
            logincheck=adminmodify(db,id,pwd,name);
        }
        json resp;
        resp["success"]=logincheck;

        res.set_content(resp.dump(),"application/json");
    });

    svr.Post("/register",[db](const httplib::Request& req,httplib::Response& res){
        json recv=json::parse(req.body);
        std::string id=recv["id"];
        std::string pwd=recv["password"];
        std::string name=recv["name"];
        std::string major=recv["major"];
        std::string grade=recv["grade"];
        bool registercheck=false;
        registercheck=studentregister(db,id,pwd,name,major,grade);
        json resp;
        resp["success"]=registercheck;

        res.set_content(resp.dump(),"application/json");
    });

    svr.Post("/select",[db](const httplib::Request& req,httplib::Response& res){
        json recv=json::parse(req.body);
        std::string courseid=recv["courseid"];
        bool selectcheck=studentselect(db,current_user_id,courseid);
        json resp;
        resp["success"]=selectcheck;
        res.set_content(resp.dump(),"application/json");
    });

    svr.Post("/drop",[db](const httplib::Request& req,httplib::Response& res){
        json recv=json::parse(req.body);
        std::string courseid=recv["courseid"];
        bool selectcheck=studentdrop(db,current_user_id,courseid);
        json resp;
        resp["success"]=selectcheck;
        res.set_content(resp.dump(),"application/json");
    });

     svr.Get("/viewselectedcourses",[db](const httplib::Request& req,httplib::Response& res){
        std::string id=current_user_id;
        std::string role=current_user_role;

        std::string tablename="Schedule"+id;
        std::string sql1="select * from "+tablename+" order by week_day asc, starting_time asc;";

        sqlite3_stmt* stmt1=nullptr;
        int rc1=sqlite3_prepare_v2(db,sql1.c_str(),-1,&stmt1,nullptr);
        if(rc1!=SQLITE_OK)
        {
            std::cerr<<"Failed to compile SQL"<<sqlite3_errmsg(db)<<std::endl;
            return;
        }
        

        json courses=json::array();

        while((rc1=sqlite3_step(stmt1))==SQLITE_ROW)
        {
            const unsigned char* courseid=sqlite3_column_text(stmt1,0);
            std::string sql="select * from Course where course_id=?;";
            sqlite3_stmt* stmt=nullptr;
            int rc=sqlite3_prepare_v2(db,sql.c_str(),-1,&stmt,nullptr);
            if(rc!=SQLITE_OK)
            {
                std::cerr<<"Failed to compile SQL"<<sqlite3_errmsg(db)<<std::endl;
                return;
            }
            std::string temp=courseid?reinterpret_cast<const char*>(courseid):"";;
            rc=sqlite3_bind_text(stmt,1,temp.c_str(),-1,SQLITE_TRANSIENT);
            if(rc!=SQLITE_OK)
            {
                std::cerr<<"Failed to bind"<<sqlite3_errmsg(db)<<std::endl;
                sqlite3_finalize(stmt);
                return;
            }
            rc=sqlite3_step(stmt);


            const unsigned char* coursename=sqlite3_column_text(stmt,1);
            const unsigned char* coursecredits=sqlite3_column_text(stmt,2);
            const unsigned char* courseschool=sqlite3_column_text(stmt,3);
            const unsigned char* coursestartingtime=sqlite3_column_text(stmt,4);
            const unsigned char* courseendingtime=sqlite3_column_text(stmt,5);
            const unsigned char* courseweekday=sqlite3_column_text(stmt,6);
            const unsigned char* courselocation=sqlite3_column_text(stmt,7);
            const unsigned char* coursestatus=sqlite3_column_text(stmt1,4);
            json course;
            course["id"]=courseid?reinterpret_cast<const char*>(courseid):"";
            course["name"]=coursename?reinterpret_cast<const char*>(coursename):"";
            course["credits"]=coursecredits?reinterpret_cast<const char*>(coursecredits):"";
            course["school"]=courseschool?reinterpret_cast<const char*>(courseschool):"";
            course["startingtime"]=coursestartingtime?reinterpret_cast<const char*>(coursestartingtime):"";
            course["endingtime"]=courseendingtime?reinterpret_cast<const char*>(courseendingtime):"";
            course["weekday"]=courseweekday?reinterpret_cast<const char*>(courseweekday):"";
            course["location"]=courselocation?reinterpret_cast<const char*>(courselocation):"";
            course["status"]=coursestatus?reinterpret_cast<const char*>(coursestatus):"";
            courses.push_back(course);
            sqlite3_finalize(stmt);
        }
            json resp;
            resp["courses"]=courses;

            res.set_content(resp.dump(),"application/json");
        sqlite3_finalize(stmt1);
    });

    svr.Post("/addcourse",[db](const httplib::Request& req,httplib::Response& res){
        json recv=json::parse(req.body);
        std::string course_id=recv["course_id"];
        std::string course_name=recv["course_name"];
        std::string credits=recv["credits"];
        std::string department=recv["department"];
        std::string starting_time=recv["starting_time"];
        std::string ending_time=recv["ending_time"];
        std::string week_day=recv["week_day"];
        std::string class_location=recv["class_location"];
        bool selectcheck=adminadd(db,course_id,course_name,credits,department,starting_time,ending_time,week_day,class_location);
        json resp;
        resp["success"]=selectcheck;
        res.set_content(resp.dump(),"application/json");
    });

    svr.Post("/deletecourse",[db](const httplib::Request& req,httplib::Response& res){
        json recv=json::parse(req.body);
        std::string courseid=recv["courseid"];
        bool selectcheck=admindelete(db,courseid);
        json resp;
        resp["success"]=selectcheck;
        res.set_content(resp.dump(),"application/json");
    });

    svr.Post("/progress",[db](const httplib::Request& req,httplib::Response& res){
        json recv=json::parse(req.body);
        std::string course_id=recv["course_id"];
        std::string student_id=recv["student_id"];
        std::string status=recv["status"];
        bool progresscheck=adminprogress(db,student_id,course_id,status);
        json resp;
        resp["success"]=progresscheck;
        res.set_content(resp.dump(),"application/json");
    });

    svr.Get("/pendingrequest",[db](const httplib::Request& req,httplib::Response& res){
        const char* sqlstudent=
        "select * " 
        "from Student " ;
        sqlite3_stmt* stmtstudent=nullptr;
        int rcstudent=sqlite3_prepare_v2(db,sqlstudent,-1,&stmtstudent,nullptr);
        if(rcstudent!=SQLITE_OK)
        {
            std::cerr<<"Failed to compile studentSQL"<<sqlite3_errmsg(db)<<std::endl;
            return;
        }

        json courses=json::array();
        while((rcstudent=sqlite3_step(stmtstudent))==SQLITE_ROW)
        {
        const unsigned char* studentid1=sqlite3_column_text(stmtstudent,0);
        std::string sid=studentid1?reinterpret_cast<const char*>(studentid1):"";
        std::string role="student";

        std::string tablename="Schedule"+sid;
        std::string sql1="select * from "+tablename+" order by week_day asc, starting_time asc;";

        sqlite3_stmt* stmt1=nullptr;
        int rc1=sqlite3_prepare_v2(db,sql1.c_str(),-1,&stmt1,nullptr);
        if(rc1!=SQLITE_OK)
        {
            std::cerr<<"Failed to compile SQL"<<sqlite3_errmsg(db)<<std::endl;
            return;
        }
        


        while((rc1=sqlite3_step(stmt1))==SQLITE_ROW)
        {
            const unsigned char* courseid=sqlite3_column_text(stmt1,0);
            std::string sql="select * from Course where course_id=?;";
            sqlite3_stmt* stmt=nullptr;
            int rc=sqlite3_prepare_v2(db,sql.c_str(),-1,&stmt,nullptr);
            if(rc!=SQLITE_OK)
            {
                std::cerr<<"Failed to compile SQL"<<sqlite3_errmsg(db)<<std::endl;
                return;
            }
            std::string temp=courseid?reinterpret_cast<const char*>(courseid):"";;
            rc=sqlite3_bind_text(stmt,1,temp.c_str(),-1,SQLITE_TRANSIENT);
            if(rc!=SQLITE_OK)
            {
                std::cerr<<"Failed to bind"<<sqlite3_errmsg(db)<<std::endl;
                sqlite3_finalize(stmt);
                return;
            }
            rc=sqlite3_step(stmt);


            const unsigned char* coursename=sqlite3_column_text(stmt,1);
            const unsigned char* coursecredits=sqlite3_column_text(stmt,2);
            const unsigned char* courseschool=sqlite3_column_text(stmt,3);
            const unsigned char* coursestartingtime=sqlite3_column_text(stmt,4);
            const unsigned char* courseendingtime=sqlite3_column_text(stmt,5);
            const unsigned char* courseweekday=sqlite3_column_text(stmt,6);
            const unsigned char* courselocation=sqlite3_column_text(stmt,7);
            const unsigned char* coursestatus=sqlite3_column_text(stmt1,4);
            json course;
            course["sid"]=sid;
            course["id"]=courseid?reinterpret_cast<const char*>(courseid):"";
            course["status"]=coursestatus?reinterpret_cast<const char*>(coursestatus):"";
            if(course["status"]=="pending")
            {
                courses.push_back(course);
            }
            
            sqlite3_finalize(stmt);
        }
            

        sqlite3_finalize(stmt1);
        }
        
        sqlite3_finalize(stmtstudent);
            json resp;
            resp["courses"]=courses;
            res.set_content(resp.dump(),"application/json");
    });

    svr.Get("/coursestatus",[db](const httplib::Request& req,httplib::Response& res){
        
        std::string id=current_user_id;
        std::string role=current_user_role;

        const char* sql=
        "select * from Course";

        sqlite3_stmt* stmt=nullptr;
        int rc=sqlite3_prepare_v2(db,sql,-1,&stmt,nullptr);
        if(rc!=SQLITE_OK)
        {
            std::cerr<<"Failed to compile SQL"<<sqlite3_errmsg(db)<<std::endl;
            return;
        }
        

        json courses=json::array();

        while((rc=sqlite3_step(stmt))==SQLITE_ROW)
        {
            const unsigned char* courseid=sqlite3_column_text(stmt,0);
            const unsigned char* coursename=sqlite3_column_text(stmt,1);
            const unsigned char* coursecredits=sqlite3_column_text(stmt,2);
            const unsigned char* courseschool=sqlite3_column_text(stmt,3);
            const unsigned char* coursestartingtime=sqlite3_column_text(stmt,4);
            const unsigned char* courseendingtime=sqlite3_column_text(stmt,5);
            const unsigned char* courseweekday=sqlite3_column_text(stmt,6);
            const unsigned char* courselocation=sqlite3_column_text(stmt,7);
            const unsigned char* studentnumber=sqlite3_column_text(stmt,8);
            json course;
            course["id"]=courseid?reinterpret_cast<const char*>(courseid):"";
            course["name"]=coursename?reinterpret_cast<const char*>(coursename):"";
            course["credits"]=coursecredits?reinterpret_cast<const char*>(coursecredits):"";
            course["school"]=courseschool?reinterpret_cast<const char*>(courseschool):"";
            course["startingtime"]=coursestartingtime?reinterpret_cast<const char*>(coursestartingtime):"";
            course["endingtime"]=courseendingtime?reinterpret_cast<const char*>(courseendingtime):"";
            course["weekday"]=courseweekday?reinterpret_cast<const char*>(courseweekday):"";
            course["location"]=courselocation?reinterpret_cast<const char*>(courselocation):"";
            course["studentnumber"]=studentnumber?reinterpret_cast<const char*>(studentnumber):"";
            courses.push_back(course);
        }
        sqlite3_finalize(stmt);
        json resp;
        resp["courses"]=courses;

        res.set_content(resp.dump(),"application/json");
    });

    svr.Post("/coursemodify",[db](const httplib::Request& req,httplib::Response& res){
        json recv=json::parse(req.body);
        std::string courseID=recv["courseID"];
        std::string credits=recv["credits"];
        bool logincheck=false;
        std::string coursetitle=recv["coursetitle"];
        std::string department=recv["department"];
        std::string location=recv["location"];
        logincheck=coursemodify(db,courseID,credits,coursetitle,department,location);
        
        json resp;
        resp["success"]=logincheck;

        res.set_content(resp.dump(),"application/json");
    });
    printf("listening\n");
    svr.listen("0.0.0.0",8080);
    sqlite3_close(db);
    return 0;
}