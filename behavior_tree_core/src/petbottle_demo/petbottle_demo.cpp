#include <behavior_tree.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "petbottle_demo");
    try
    {
        int TickPeriod_milliseconds = 100;


        BT::ROSAction* pick_action = new BT::ROSAction("pick");
        BT::ROSAction* search_action = new BT::ROSAction("search");
        BT::ROSCondition* lost_cond = new BT::ROSCondition("lost");
        BT::ROSCondition* moved_cond = new BT::ROSCondition("moved");

        BT::NegationNode* decorator_lost = new BT::NegationNode("decorator1");
        BT::NegationNode* decorator_moved = new BT::NegationNode("decorator2");

        BT::SequenceNode* sequence_root = new BT::SequenceNode("seq0");
        BT::SequenceNode* sequence_pick = new BT::SequenceNode("seq1");
        BT::FallbackNode* fallback_search = new BT::FallbackNode("fall1");

        sequence_root->AddChild(fallback_search);
        sequence_root->AddChild(sequence_pick);

        fallback_search->AddChild(decorator_lost);
        fallback_search->AddChild(search_action);

        sequence_pick->AddChild(decorator_moved);
        sequence_pick->AddChild(pick_action);

        decorator_lost->AddChild(lost_cond);
        decorator_moved->AddChild(moved_cond);

        sequence_root->set_reset_policy(BT::ON_FAILURE);
        Execute(sequence_root, TickPeriod_milliseconds);  // from BehaviorTree.cpp
    }
    catch (BT::BehaviorTreeException& Exception)
    {
        std::cout << Exception.what() << std::endl;
    }

    return 0;
}
