import json
import requests
from flask import Flask,request,jsonify
from openai import OpenAI

app=Flask(__name__)

OPENROUTER_API_KEY="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"
MODEL_NAME="deepseek/deepseek-v3.2"
CPP_BASE="http://localhost:8080"

client=OpenAI(
    api_key=OPENROUTER_API_KEY,
    base_url="https://openrouter.ai/api/v1"
)

def get_all_courses():
    r=requests.get(f"{CPP_BASE}/course_info",timeout=10)
    r.raise_for_status()
    return r.json()

def get_selected_courses():
    r=requests.get(f"{CPP_BASE}/viewselectedcourses",timeout=10)
    r.raise_for_status()
    return r.json()

def add_course(course_id:str):
    r=requests.post(
        f"{CPP_BASE}/select",
        json={"courseid":course_id},
        timeout=10
    )
    r.raise_for_status()
    return r.json()

def drop_course(course_id:str):
    r=requests.post(
        f"{CPP_BASE}/drop",
        json={"courseid":course_id},
        timeout=10
    )
    r.raise_for_status()
    return r.json()

TOOLS=[
    {
        "type":"function",
        "function":{
            "name":"get_all_courses",
            "description":"Get all available courses from the course system.",
            "parameters":{
                "type":"object",
                "properties":{},
                "required":[]
            }
        }
    },
    {
        "type":"function",
        "function":{
            "name":"get_selected_courses",
            "description":"Get the current selected courses of the logged-in student.",
            "parameters":{
                "type":"object",
                "properties":{},
                "required":[]
            }
        }
    },
    {
        "type":"function",
        "function":{
            "name":"add_course",
            "description":"Add a course by course ID, for example C001.",
            "parameters":{
                "type":"object",
                "properties":{
                    "course_id":{
                        "type":"string",
                        "description":"Course ID such as C001."
                    }
                },
                "required":["course_id"]
            }
        }
    },
    {
        "type":"function",
        "function":{
            "name":"drop_course",
            "description":"Drop a selected course by course ID, for example C001.",
            "parameters":{
                "type":"object",
                "properties":{
                    "course_id":{
                        "type":"string",
                        "description":"Course ID such as C001."
                    }
                },
                "required":["course_id"]
            }
        }
    }
]

def call_local_tool(name,arguments):
    if name=="get_all_courses":
        return get_all_courses()
    if name=="get_selected_courses":
        return get_selected_courses()
    if name=="add_course":
        return add_course(arguments["course_id"])
    if name=="drop_course":
        return drop_course(arguments["course_id"])
    return {"success":False,"error":f"Unknown tool: {name}"}

@app.route("/llm_agent",methods=["POST"])
def llm_agent():
    data=request.get_json(silent=True) or {}
    query=data.get("query","").strip()

    if not query:
        return jsonify({
            "success":False,
            "final_message":"Empty query."
        }),400

    messages=[
        {
            "role":"system",
            "content":(
                "You are a course planning agent.\n"
                "Use tools whenever needed.\n"
                "Never invent tool results.\n"
                "If the user wants to add or drop a course and the course ID is clear, call the tool directly.\n"
                "If the user asks about current courses, call get_selected_courses.\n"
                "If the user asks to find courses, call get_all_courses first.\n"
                "At the end, give a concise final answer."
            )
        },
        {
            "role":"user",
            "content":query
        }
    ]

    actions=[]

    try:
        while True:
            response=client.chat.completions.create(
                model=MODEL_NAME,
                messages=messages,
                tools=TOOLS,
                tool_choice="auto",
                temperature=0
            )

            message=response.choices[0].message

            if getattr(message,"tool_calls",None):
                messages.append(message)

                for tool_call in message.tool_calls:
                    tool_name=tool_call.function.name
                    arguments=json.loads(tool_call.function.arguments)

                    tool_result=call_local_tool(tool_name,arguments)

                    actions.append({
                        "tool":tool_name,
                        "arguments":arguments,
                        "result":tool_result
                    })

                    messages.append({
                        "role":"tool",
                        "tool_call_id":tool_call.id,
                        "content":json.dumps(tool_result,ensure_ascii=False)
                    })

                continue

            final_message=message.content if message.content else "No response."

            return jsonify({
                "success":True,
                "actions":actions,
                "final_message":final_message
            })

    except requests.RequestException as e:
        return jsonify({
            "success":False,
            "final_message":f"Tool server request failed: {str(e)}"
        }),500
    except Exception as e:
        return jsonify({
            "success":False,
            "final_message":f"Agent failed: {str(e)}"
        }),500

if __name__=="__main__":
    app.run(port=5000,debug=True)