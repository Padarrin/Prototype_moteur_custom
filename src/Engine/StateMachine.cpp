#include "StateMachine.h"

void StateMachine::SetAction(const std::string& _name, std::function<void()> _pOnBegin
    , std::function<void()> _pOnUpdate, std::function<void()> _pOnEnd)
{
    m_actions[_name].OnEnter = _pOnBegin;
    m_actions[_name].OnUpdate = _pOnUpdate;
    m_actions[_name].OnExit = _pOnEnd;

}

void StateMachine::SetTransition(std::vector<Condition>& _conditions, std::string& _from, std::string& _to)
{
    Transition t(_conditions, _from, _to);
    m_actions[_from].transitions.push_back(t);
}

void StateMachine::ChangeState(const std::string& _name)
{
    if (m_currentAction != nullptr && m_currentAction->OnExit != nullptr)
    {
        m_currentAction->OnExit();
    }

    m_currentAction = &m_actions[_name];
    m_currentActionName = _name;
   
    if (m_currentAction != nullptr && m_currentAction->OnEnter != nullptr)
    {
        m_currentAction->OnEnter();
    }
}

void StateMachine::Update()
{
    for (int i = 0; i < m_actions[m_currentActionName].transitions.size(); i++)
    {
        Transition transition = m_actions[m_currentActionName].transitions[i];
        for (int i = 0; i < transition.conditions.size(); i++)
        {
            bool condition = false;
            condition = transition.conditions[i].condition();

            if (condition == true)
            {
                std::string to = transition.to;
                ChangeState(to);
            }
        }
    }

    if (m_currentAction != nullptr && m_currentAction->OnUpdate != nullptr)
    {
        m_currentAction->OnUpdate();
    }
}
