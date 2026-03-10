#pragma once
#include "pch.h"

struct Condition
{
    std::function<bool()> condition;
};

class StateMachine
{
private :
    struct State 
    {
        std::function<void()> OnEnter = nullptr;
        std::function<void()> OnUpdate = nullptr;
        std::function<void()> OnExit = nullptr;
    };

    

    struct Transition
    {
        std::vector<Condition> conditions;
        std::string from;
        std::string to;
    };

    struct Action
    {
        std::function<void()> OnEnter;
        std::function<void()> OnUpdate;
        std::function<void()> OnExit;

        std::vector<Transition> transitions;
    };

    std::unordered_map<std::string, Action> m_actions;
    
    Action* m_currentAction = nullptr;


public : 

    std::string m_currentActionName;

    void SetAction(const std::string& _name, std::function<void()> _pOnBegin
        , std::function<void()> _pOnUpdate, std::function<void()> _pOnEnd);

    void SetTransition(std::vector<Condition>& _conditions, std::string& _from, std::string& _to);

    void ChangeState(const std::string& _name);
    void Update();

  

};

