//
// CS104 Heap test suite
//

// make public for access to internal data
//#define private public
//#define protected public
// make private = public
//#define CS104TEST
#include <labellist.h>
//#undef private
//#undef protected



#include <gtest/gtest.h>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <utility>
#include <string>


using namespace std;

typedef std::vector<LabelList::MsgType> MsgVec;

// Creates a standard list pattern that we will use for testing
void setupList1(
    LabelList& list1, LabelList::MsgType msgs[], LabelList::MsgToken tokens[], size_t numMsgs, bool applyChecks = true);

// Compares the messages in a list with those that are expected
testing::AssertionResult compareList(const MsgVec& exp, const LabelList& theList, const LabelList::LabelType& label);

void setupList1(
    LabelList& list1, LabelList::MsgType msgs[], LabelList::MsgToken tokens[], size_t numMsgs, bool applyChecks)
{
    for(size_t i = 0U; i != numMsgs; i++)
    {
        tokens[i] = list1.add(msgs[i]);
    }

    list1.label(tokens[1], "la");
    if(applyChecks) {EXPECT_TRUE(compareList({"m1"}, list1, "la"));}

    list1.label(tokens[1], "lb");
    if(applyChecks) {EXPECT_TRUE(compareList({"m1"}, list1, "lb"));}

    list1.label(tokens[1], "lc");
    if(applyChecks) {EXPECT_TRUE(compareList({"m1"}, list1, "lc"));}

    list1.label(tokens[4], "ld");
    if(applyChecks) {EXPECT_TRUE(compareList({"m4"}, list1, "ld"));}
    list1.label(tokens[0], "ld");
    if(applyChecks) {EXPECT_TRUE(compareList({"m0", "m4"}, list1, "ld"));}
    list1.label(tokens[5], "ld");
    if(applyChecks) {EXPECT_TRUE(compareList({"m0", "m4", "m5"}, list1, "ld"));}
    list1.label(tokens[2], "ld");
    if(applyChecks) {EXPECT_TRUE(compareList({"m0", "m2", "m4", "m5"}, list1, "ld"));}

    if(applyChecks) {
        EXPECT_TRUE(compareList({"m1"}, list1, "la"));
        EXPECT_TRUE(compareList({"m1"}, list1, "lb"));
        EXPECT_TRUE(compareList({"m1"}, list1, "lc"));
    }
}

testing::AssertionResult compareList(const MsgVec& exp, const LabelList& theList, const LabelList::LabelType& label)
{
 	stringstream ssErr, ssData;
	bool err = false;

    theList.print(ssData, label);
    MsgVec actual;
    LabelList::MsgType tempMsg;
    while(ssData >> tempMsg)
    {
        actual.push_back(tempMsg);
    }  
    if(exp.size() != actual.size())  
    {
        ssErr << "Label " << label << " has " << actual.size() << " messages, when " << exp.size() << " is expected.";
        cout << ssErr.str() << endl;
        return testing::AssertionFailure() << ssErr.str();  
    }
    for(size_t i = 0U; i < exp.size(); i++)
    {
        if(exp[i] != actual[i])
        {
            err = true;
            ssErr << "Element " << i << "(Expected: " << exp[i] << " / Actual: " << actual[i] << ")\n";
        }
    }
    if(err)
    {
        cout << ssErr.str() << endl;
        return testing::AssertionFailure() << ssErr.str();  
    }
	return testing::AssertionSuccess();
}


TEST(LabelList, Nominal)
{
    LabelList list1;
    const size_t NUM_MSGS = 3;
    LabelList::MsgType msgs[NUM_MSGS] = {"a1", "b1", "a2"};
    LabelList::MsgToken tokens[NUM_MSGS];
    for(size_t i = 0U; i != NUM_MSGS; i++)
    {
        tokens[i] = list1.add(msgs[i]);
    }
    // You can uncomment these print statement to help debug.
    // Feel free to add your own print/cout statements to help debug.
    //list1.print(std::cout, "all");
    list1.label(tokens[0], "la");
    //list1.print(std::cout, "la");
    list1.label(tokens[1], "lb");
    list1.label(tokens[2], "la");
    //list1.print(std::cout, "la");
    LabelList::MsgToken tt1 = list1.find(1, "la");
    EXPECT_EQ(tokens[2].msg(), tt1.msg());
    LabelList::MsgToken tt2 = list1.find(0, "lb");
    EXPECT_EQ(tokens[1].msg(), tt2.msg());

    EXPECT_EQ(tokens[1], tt1.prev("all"));
    EXPECT_EQ(tokens[0], tt1.prev("all"));
    EXPECT_EQ(tokens[2], tt1.next("la"));
    EXPECT_EQ(list1.end(), tt1.next("la"));

    LabelList::MsgToken tt2Copy = tt2;
    EXPECT_EQ(list1.end(), tt2Copy.prev("lb"));
    EXPECT_EQ(list1.end(), tt2.next("lb"));

}

TEST(EmptyList, EmptyAndSize)
{
    EXPECT_NO_THROW(LabelList());
    LabelList list1;
    EXPECT_EQ(true, list1.empty());
    EXPECT_EQ(0U, list1.size());
}
TEST(EmptyList, OperationsOnEmptyList)
{
    LabelList list1;
    EXPECT_EQ(list1.end(), list1.find(0, "all"));
    EXPECT_EQ(list1.end(), list1.find("content", "all"));
    EXPECT_NO_THROW(list1.clear());
    EXPECT_NO_THROW(list1.remove(list1.end()));

    std::stringstream ss;    
    EXPECT_NO_THROW(list1.print(ss,"all"));
    EXPECT_EQ("\n", ss.str());

}

TEST(Exceptions, Find)
{
    LabelList list1;
    // Invalid labels for find
    EXPECT_THROW(list1.find(0, "label1"), std::out_of_range);
    EXPECT_THROW(list1.find("content", "label1"), std::out_of_range);
}
TEST(Exceptions, Label)
{
    LabelList list1;
    // Invalid arguments to label
    EXPECT_THROW(list1.label(list1.end(), "la"), std::invalid_argument);
}
TEST(Exceptions, Print)
{
    LabelList list1;
    // Invalid argument to print
    stringstream ss1;
    EXPECT_THROW(list1.print(ss1, "invalid_label"), std::out_of_range);
}
TEST(Exceptions, Unlabel)
{
    LabelList list1;
    LabelList::MsgToken t1 = list1.add("Msg1");
    list1.label(t1,"la");
    // Invalid argument to unlabel
    EXPECT_THROW(list1.unlabel(t1, "invalid_label"), std::out_of_range);
    EXPECT_THROW(list1.unlabel(list1.end(), "la"), std::invalid_argument);
    EXPECT_THROW(list1.unlabel(list1.end(), "all"), std::invalid_argument);

}

TEST(Label, Label1)
{
    LabelList list1;
    LabelList::MsgType msgs[] = {"m0", "m1", "m2", "m3", "m4", "m5"};
    const size_t NUM_MSGS = sizeof(msgs)/sizeof(LabelList::MsgType);
    LabelList::MsgToken tokens[NUM_MSGS];

    setupList1(list1, msgs, tokens, NUM_MSGS);

}

TEST(Label, Label2)
{
    // Same operations from Label1 and then new modifications below
    LabelList list1;
    LabelList::MsgType msgs[] = {"m0", "m1", "m2", "m3", "m4", "m5"};
    const size_t NUM_MSGS = sizeof(msgs)/sizeof(LabelList::MsgType);
    LabelList::MsgToken tokens[NUM_MSGS];

    setupList1(list1, msgs, tokens, NUM_MSGS);

    // ============================
    // New operations for this test
    // ============================
    list1.label(tokens[2], "la");
    EXPECT_TRUE(compareList({"m1", "m2"}, list1, "la"));

    list1.label(tokens[3], "lb");
    EXPECT_TRUE(compareList({"m1", "m3"}, list1, "lb"));

    EXPECT_TRUE(compareList({"m1"}, list1, "lc"));
    EXPECT_TRUE(compareList({"m0", "m2", "m4", "m5"}, list1, "ld"));
}

TEST(Find, FindEmpty)
{
    LabelList list1;

    LabelList::MsgToken t = list1.find("missing", "all");
    EXPECT_EQ(list1.end(), t);
}

TEST(Find, FindViaMessage)
{
    LabelList list1;
    LabelList::MsgType msgs[] = {"m0", "m1", "m2", "m3", "m4", "m5"};
    const size_t NUM_MSGS = sizeof(msgs)/sizeof(LabelList::MsgType);
    LabelList::MsgToken tokens[NUM_MSGS];

    setupList1(list1, msgs, tokens, NUM_MSGS, false);

    LabelList::MsgToken t;
    t = list1.find("m2", "ld");
    EXPECT_EQ(tokens[2], t);

    t = list1.find("m5", "ld");
    EXPECT_EQ(tokens[5], t);

    t = list1.find("missing", "ld");
    EXPECT_EQ(list1.end(), t);

    t = list1.find("m1", "la");
    EXPECT_EQ(tokens[1], t);

    t = list1.find("missing", "la");
    EXPECT_EQ(list1.end(), t);
}

TEST(Find, FindViaIndex)
{
    LabelList list1;
    LabelList::MsgType msgs[] = {"m0", "m1", "m2", "m3", "m4", "m5"};
    const size_t NUM_MSGS = sizeof(msgs)/sizeof(LabelList::MsgType);
    LabelList::MsgToken tokens[NUM_MSGS];

    setupList1(list1, msgs, tokens, NUM_MSGS, false);

    LabelList::MsgToken t;
    t = list1.find(1, "ld");
    EXPECT_EQ(tokens[2], t);

    t = list1.find(3, "ld");
    EXPECT_EQ(tokens[5], t);

    t = list1.find(10, "ld");
    EXPECT_EQ(list1.end(), t);

    t = list1.find(0, "la");
    EXPECT_EQ(tokens[1], t);

    t = list1.find(10, "la");
    EXPECT_EQ(list1.end(), t);
}

TEST(Unlabel, UnlabelBadLabel)
{
    LabelList list1;
    LabelList::MsgType msgs[] = {"m0", "m1", "m2"};
    const size_t NUM_MSGS = sizeof(msgs)/sizeof(LabelList::MsgType);
    LabelList::MsgToken tokens[NUM_MSGS];
    for(size_t i = 0U; i != NUM_MSGS; i++)
    {
        tokens[i] = list1.add(msgs[i]);
    }
    list1.label(tokens[1], "la");
    list1.label(tokens[1], "lb");
    list1.label(tokens[1], "lc");

    list1.label(tokens[0], "ld");
    list1.label(tokens[2], "ld");

    // Shouldn't be able to unlabel a msg from the "all" label
    EXPECT_NO_THROW(list1.unlabel(tokens[0], "all"));
    EXPECT_TRUE(compareList({"m0", "m1", "m2"}, list1, "all"));

    // Shouldn't be able to unlabel a msg that isn't a member of that label
    EXPECT_NO_THROW(list1.unlabel(tokens[0], "la"));
    EXPECT_TRUE(compareList({"m0", "m1", "m2"}, list1, "all"));
    EXPECT_TRUE(compareList({"m1"}, list1, "la"));
    EXPECT_TRUE(compareList({"m0", "m2"}, list1, "ld"));

}

TEST(Unlabel, Unlabel1)
{
    LabelList list1;
    LabelList::MsgType msgs[] = {"m0", "m1", "m2", "m3", "m4", "m5"};
    const size_t NUM_MSGS = sizeof(msgs)/sizeof(LabelList::MsgType);
    LabelList::MsgToken tokens[NUM_MSGS];

    setupList1(list1, msgs, tokens, NUM_MSGS, false);

    EXPECT_TRUE(compareList({"m0", "m2", "m4", "m5"}, list1, "ld"));

    list1.unlabel(tokens[1], "lb");
    EXPECT_TRUE(compareList({}, list1, "lb"));

    list1.unlabel(tokens[2], "ld");
    EXPECT_TRUE(compareList({"m0", "m4", "m5"}, list1, "ld"));

    list1.unlabel(tokens[0], "ld");
    EXPECT_TRUE(compareList({"m4", "m5"}, list1, "ld"));

    list1.unlabel(tokens[5], "ld");
    EXPECT_TRUE(compareList({"m4"}, list1, "ld"));

    list1.unlabel(tokens[4], "ld");
    EXPECT_TRUE(compareList({}, list1, "ld"));

    EXPECT_TRUE(compareList({"m1"}, list1, "la"));
    EXPECT_TRUE(compareList({"m1"}, list1, "lc"));
}

TEST(Unlabel, UnlabelRemove)
{
    LabelList list1;
    LabelList::MsgType msgs[] = {"m0", "m1", "m2", "m3", "m4", "m5"};
    const size_t NUM_MSGS = sizeof(msgs)/sizeof(LabelList::MsgType);
    LabelList::MsgToken tokens[NUM_MSGS];
    for(size_t i = 0U; i != NUM_MSGS; i++)
    {
        tokens[i] = list1.add(msgs[i]);
    }
    list1.label(tokens[1], "la");
    list1.label(tokens[1], "lb");
    list1.label(tokens[1], "lc");

    list1.label(tokens[4], "la");
    list1.label(tokens[4], "lb");
    list1.label(tokens[4], "lc");
    

    list1.label(tokens[0], "ld");
    list1.label(tokens[5], "ld");

    // Unlabel then add a new label to that same node
    list1.unlabel(tokens[1], "lc");
    list1.label(tokens[1], "ld");

    EXPECT_TRUE(compareList({"m4"}, list1, "lc"));
    EXPECT_TRUE(compareList({"m0", "m1", "m5"}, list1, "ld"));


    // Unlink a node and entirely remove a node we've unlinked previously
    list1.unlabel(tokens[4], "la");
    list1.remove(tokens[1]);
    EXPECT_TRUE(compareList({}, list1, "la"));
    EXPECT_TRUE(compareList({"m4"}, list1, "lb"));
    EXPECT_TRUE(compareList({"m4"}, list1, "lc"));
    EXPECT_TRUE(compareList({"m0", "m5"}, list1, "ld"));
}

TEST(Label, AddRemoveAfterLabel)
{
    LabelList list1;
    LabelList::MsgType msgs[] = {"m0", "m1", "m2", "m3", "m4", "m5"};
    const size_t NUM_MSGS = sizeof(msgs)/sizeof(LabelList::MsgType);
    LabelList::MsgToken tokens[NUM_MSGS];

    setupList1(list1, msgs, tokens, NUM_MSGS, false);

    list1.remove(tokens[1]);

    EXPECT_TRUE(compareList({}, list1, "la"));
    EXPECT_TRUE(compareList({}, list1, "lb"));
    EXPECT_TRUE(compareList({}, list1, "lc"));
    EXPECT_TRUE(compareList({"m0", "m2", "m4", "m5"}, list1, "ld"));

    list1.remove(tokens[2]);
    EXPECT_TRUE(compareList({"m0", "m4", "m5"}, list1, "ld"));

    list1.remove(tokens[0]);
    EXPECT_TRUE(compareList({"m4", "m5"}, list1, "ld"));

    LabelList::MsgToken t6 = list1.add("m6");
    LabelList::MsgToken t7 = list1.add("m7");

    list1.label(t6, "la");
    list1.label(t7, "ld");

    EXPECT_TRUE(compareList({"m6"}, list1, "la"));
    EXPECT_TRUE(compareList({}, list1, "lb"));
    EXPECT_TRUE(compareList({}, list1, "lc"));
    EXPECT_TRUE(compareList({"m4", "m5", "m7"}, list1, "ld"));

    list1.label(tokens[3], "la");
    EXPECT_TRUE(compareList({"m3", "m6"}, list1, "la"));

    list1.label(tokens[4], "lb");
    EXPECT_TRUE(compareList({"m4"}, list1, "lb"));

    EXPECT_TRUE(compareList({}, list1, "lc"));
    EXPECT_TRUE(compareList({"m4", "m5", "m7"}, list1, "ld"));
}

TEST(MsgToken, ComparisonBool)
{
    LabelList list1;
    LabelList::MsgType msgs[] = {"m0", "m1", "m2", "m3", "m4", "m5", "m5"};
    const size_t NUM_MSGS = sizeof(msgs)/sizeof(LabelList::MsgType);
    LabelList::MsgToken tokens[NUM_MSGS];
    for(size_t i = 0U; i != NUM_MSGS; i++)
    {
        tokens[i] = list1.add(msgs[i]);
    }
    list1.label(tokens[2], "la");
    list1.label(tokens[4], "la");

    // Comparison and bool operator
    LabelList::MsgToken ta = list1.find("m2", "all");
    EXPECT_TRUE(ta);
    EXPECT_FALSE(list1.end());

    EXPECT_TRUE(tokens[2] == ta);
    EXPECT_FALSE(tokens[2] != ta);

    EXPECT_TRUE(tokens[3] != ta);
    EXPECT_FALSE(tokens[3] == ta);

    //Compare two different tokens with same msg
    list1.label(tokens[5], "lb");
    list1.label(tokens[6], "lc");

    LabelList::MsgToken tb = list1.find("m5", "lb");
    LabelList::MsgToken tc = list1.find("m5", "lc");

    EXPECT_TRUE(tb != tc);
    EXPECT_FALSE(tb == tc);
    EXPECT_TRUE(tokens[5] == tb);
    EXPECT_TRUE(tokens[6] == tc);

    //Add two msgnodes with same text. Label with same label. Find by text, get first in the list. 
    //Remove the first one. Find again, msgnode added later should be returend this time.
    list1.label(tokens[6], "lb");

    LabelList::MsgToken tb1 = list1.find("m5", "lb");
    EXPECT_TRUE(tokens[5] == tb1);

    list1.remove(tb1);

    LabelList::MsgToken tb2 = list1.find("m5", "lb");
    EXPECT_TRUE(tokens[6] == tb2);

}

TEST(MsgToken, Label)
{
    LabelList list1;
    LabelList::MsgType msgs[] = {"m0", "m1", "m2", "m3", "m4", "m5"};
    const size_t NUM_MSGS = sizeof(msgs)/sizeof(LabelList::MsgType);
    LabelList::MsgToken tokens[NUM_MSGS];
    for(size_t i = 0U; i != NUM_MSGS; i++)
    {
        tokens[i] = list1.add(msgs[i]);
    }
    list1.label(tokens[2], "la");
    list1.label(tokens[4], "la");

    // Label Member
    EXPECT_TRUE(tokens[2].isLabelMember("la"));
    EXPECT_FALSE(tokens[3].isLabelMember("la"));
}

TEST(MsgToken, Msg)
{
    LabelList list1;
    LabelList::MsgType msgs[] = {"m0", "m1", "m2", "m3", "m4", "m5"};
    const size_t NUM_MSGS = sizeof(msgs)/sizeof(LabelList::MsgType);
    LabelList::MsgToken tokens[NUM_MSGS];
    for(size_t i = 0U; i != NUM_MSGS; i++)
    {
        tokens[i] = list1.add(msgs[i]);
    }
    list1.label(tokens[2], "la");
    list1.label(tokens[4], "la");

    // Message Accessor
    const LabelList::MsgType& cmsg = tokens[2].msg();
    EXPECT_EQ("m2", cmsg);
    tokens[2].msg() = "new-m2";
    EXPECT_EQ("new-m2", tokens[2].msg());
    EXPECT_THROW(list1.end().msg(), std::logic_error);
}
TEST(MsgToken, Ostream)
{
    LabelList list1;
    LabelList::MsgType msgs[] = {"m0", "m1", "m2", "m3", "m4", "m5"};
    const size_t NUM_MSGS = sizeof(msgs)/sizeof(LabelList::MsgType);
    LabelList::MsgToken tokens[NUM_MSGS];
    for(size_t i = 0U; i != NUM_MSGS; i++)
    {
        tokens[i] = list1.add(msgs[i]);
    }
    list1.label(tokens[2], "la");
    list1.label(tokens[4], "la");

    // Ostream operator
    stringstream ss;
    ss << list1.end();
    EXPECT_EQ("", ss.str());
    ss << tokens[4];
    EXPECT_EQ("m4", ss.str());
}

TEST(MsgToken, NextPrev)
{
    LabelList list1;
    LabelList::MsgType msgs[] = {"m0", "m1", "m2", "m3", "m4", "m5"};
    const size_t NUM_MSGS = sizeof(msgs)/sizeof(LabelList::MsgType);
    LabelList::MsgToken tokens[NUM_MSGS];
    for(size_t i = 0U; i != NUM_MSGS; i++)
    {
        tokens[i] = list1.add(msgs[i]);
    }
    list1.label(tokens[2], "la");
    list1.label(tokens[4], "la");

    // Next and previous
    LabelList::MsgToken ta = list1.find("m2","la");
    ta.next("all");
    EXPECT_EQ(tokens[3], ta);
    ta.prev("all");
    EXPECT_EQ(tokens[2], ta);

    ta.prev("all");
    ta.prev("all");
    ta.prev("all");
    EXPECT_EQ(list1.end(), ta);

    ta = tokens[2];
    ta.next("la");
    EXPECT_EQ(tokens[4], ta);
    ta.next("la");
    EXPECT_EQ(list1.end(), ta);
}

TEST(Remove, UnlinkNodeLabel)
{
    LabelList lst;
    std::set<LabelList::MsgType> toLabel = {"a", "b", "d"};

    // {a, b, d} should have label L
    for (auto s : {"a", "b", "c", "d"}) {
        const auto t = lst.add(s);
        if (toLabel.count(s))
            lst.label(t, "L");
    }

    // find and remove b
    auto t = lst.find("b", "L");
    lst.remove(t);

    // {a, d} remaining for label L
    EXPECT_TRUE(compareList({"a", "d"}, lst, "L"));

    // find c and label it with L
    t = lst.find("c", "all");
    lst.label(t, "L");

    EXPECT_TRUE(compareList({"a", "c", "d"}, lst, "L"));
}
#ifdef CS104_RANDOMIZED_TESTING

TEST(Random, Random)
{
    // This test is disabled for now, since in the case of failure it can be very hard to find the issue.
    // Replaced with more compact tests that expose common problems.
    //GTEST_SKIP();

    constexpr bool verbose = false;
    if (!verbose)
        std::cout.setstate(std::ios_base::failbit);

    enum Op {
        ADD = 0,
        REMOVE,
        LABEL,
        UNLABEL,
        FIND,
        FIND_AND_REMOVE,

        LAST_OP = FIND_AND_REMOVE,
        NUM_OPS = LAST_OP + 1,
    };

    const auto nIter = 300000;

    std::mt19937 rng(42);  // deterministic pRNG
    std::uniform_int_distribution<std::mt19937::result_type> randomOp(0, LAST_OP),
                                                             randomMsgChar(int('A'),int('Z')),
                                                             randomLabelChar(int('A'), int('Z'));

    LabelList lst;
    LabelList::MsgToken t = lst.end();

    std::vector<int> opCnt(NUM_OPS, 0);

    auto genRandomStr = [&rng](decltype(randomMsgChar) &distribution, int len) -> std::string
    {
        LabelList::MsgType s(len, '\000');
        for (char &c : s)
            c = char(distribution(rng));
        return s;
    };

    auto randMsg = [&](){ return genRandomStr(randomMsgChar, 1); };
    auto randLabel = [&](){ return genRandomStr(randomLabelChar, 2); };

    for (int i = 0; i < nIter; ++i)
    {
        const auto op = randomOp(rng);
        const auto msg = randMsg(), label = randLabel();

        switch (op) {
            case ADD:
                std::cout << "Add " + msg << std::endl;
                t = lst.add(msg);
                ++opCnt[op];
                break;
            case REMOVE:
                if (t != lst.end())
                    std::cout << "Remove " << t.msg() << std::endl;

                lst.remove(t);
                t = lst.end();
                ++opCnt[op];
                break;
            case LABEL:
                if (t != lst.end()) {
                    std::cout << "Label " << t.msg() << " " << label << std::endl;
                    lst.label(t, label);
                    ++opCnt[op];
                }
                break;
            case UNLABEL:
                if (t != lst.end()) {
                    try {
                        std::cout << "Unlabel " << t.msg() << " " << label << std::endl;
                        lst.unlabel(t, label);
                        ++opCnt[op];
                    }
                    catch (std::out_of_range &) {}
                }
                break;
            case FIND:
                try {
                    t = lst.find(msg, label);
                    if (t != lst.end()) {
                        std::cout << "Find " << msg << " " << label << std::endl;
                        ++opCnt[op];
                    }
                }
                catch (std::out_of_range &) {}
                break;
            case FIND_AND_REMOVE:
                try {
                    t = lst.find(msg, label);
                    if (t != lst.end()) {
                        std::cout << "Find and remove " << msg << " " << label << std::endl;
                        lst.remove(t);
                        t = lst.end();
                        ++opCnt[op];
                    }
                }
                catch (std::out_of_range &) {}
                break;
            default:
                break;
        }
    }

    if (!verbose)
        std::cout.clear();

    std::cout << "Random test executed: \n"
              << "add(): " << opCnt[0] << " times\n"
              << "remove(): " << opCnt[1] << " times\n"
              << "label(): " << opCnt[2] << " times\n"
              << "unlabel(): " << opCnt[3] << " times\n"
              << "successful find(): " << opCnt[4] << " times\n"
              << "find() followed by remove(): " << opCnt[5] << " times\n";

    lst.clear();
    EXPECT_TRUE(lst.empty());

    std::cout << "Random test done!\n";
}

#endif