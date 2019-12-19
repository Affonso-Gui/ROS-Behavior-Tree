#include <behavior_tree.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "petbottle_demo");
    try
    {
        int TickPeriod_milliseconds = 1000;


        BT::ROSAction* pick_action = new BT::ROSAction("pick");
        BT::ROSAction* search_action = new BT::ROSAction("search");
        BT::ROSAction* finish_action = new BT::ROSAction("finish");

        BT::ROSCondition* finishing_cond = new BT::ROSCondition("finishing");
        BT::ROSCondition* lost_cond = new BT::ROSCondition("lost");
        BT::ROSCondition* grasped_cond = new BT::ROSCondition("grasped");
        BT::ROSCondition* moved_cond = new BT::ROSCondition("moved");

        BT::NegationNode* decorator_lost = new BT::NegationNode("decorator1");
        BT::NegationNode* decorator_moved = new BT::NegationNode("decorator2");

        BT::SequenceNode* sequence_root = new BT::SequenceNode("seq_root");
        BT::SequenceNode* sequence_pick = new BT::SequenceNode("seq_pick");
        BT::FallbackNode* fallback_pick = new BT::FallbackNode("fall_pick");
        BT::FallbackNode* fallback_search = new BT::FallbackNode("fall_search");

        sequence_root->AddChild(fallback_search);
        sequence_root->AddChild(fallback_pick);
        sequence_root->AddChild(finish_action);

        fallback_search->AddChild(finishing_cond);
        fallback_search->AddChild(decorator_lost);
          decorator_lost->AddChild(lost_cond);
        fallback_search->AddChild(search_action);

        fallback_pick->AddChild(grasped_cond);
        fallback_pick->AddChild(sequence_pick);
          sequence_pick->AddChild(decorator_moved);
            decorator_moved->AddChild(moved_cond);
          sequence_pick->AddChild(pick_action);

        sequence_root->set_reset_policy(BT::ON_FAILURE);
        Execute(sequence_root, TickPeriod_milliseconds);  // from BehaviorTree.cpp
    }
    catch (BT::BehaviorTreeException& Exception)
    {
        std::cout << Exception.what() << std::endl;
    }

    return 0;
}
