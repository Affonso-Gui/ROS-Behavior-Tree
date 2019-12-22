#include <behavior_tree.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "petbottle_demo");
    try
    {
        int TickPeriod_milliseconds = 100;


        BT::ROSAction* pick_action = new BT::ROSAction("pick");
        BT::ROSAction* search_action = new BT::ROSAction("search");
        BT::ROSAction* finish_action = new BT::ROSAction("finish");

        BT::ROSCondition* found_cond = new BT::ROSCondition("found");
        BT::ROSCondition* grasped_cond = new BT::ROSCondition("grasped");
        BT::ROSCondition* moved_cond = new BT::ROSCondition("moved");

        BT::NegationNode* decorator_moved = new BT::NegationNode("decorator2");

        BT::SequenceNode* sequence_finish = new BT::SequenceNode("seq_finish");
        BT::SequenceNode* sequence_pick = new BT::SequenceNode("seq_pick");
        BT::SequenceNode* sequence_pick_moved = new BT::SequenceNode("seq_pick_moved");

        BT::FallbackNode* fallback_grasped = new BT::FallbackNode("fall_grasped");
        BT::FallbackNode* fallback_found = new BT::FallbackNode("fall_found");

        sequence_finish->AddChild(fallback_grasped);
        sequence_finish->AddChild(finish_action);

        fallback_grasped->AddChild(grasped_cond);
        fallback_grasped->AddChild(sequence_pick);

        sequence_pick->AddChild(fallback_found);
        sequence_pick->AddChild(sequence_pick_moved);

        fallback_found->AddChild(found_cond);
        fallback_found->AddChild(search_action);

        sequence_pick_moved->AddChild(decorator_moved);
          decorator_moved->AddChild(moved_cond);
        sequence_pick_moved->AddChild(pick_action);

        sequence_finish->set_reset_policy(BT::ON_FAILURE);
        Execute(sequence_finish, TickPeriod_milliseconds);  // from BehaviorTree.cpp
    }
    catch (BT::BehaviorTreeException& Exception)
    {
        std::cout << Exception.what() << std::endl;
    }

    return 0;
}
