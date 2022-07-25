#include "labellist.h"
#include <cassert>



// Complete - Do not alter
LabelList::MsgNode::MsgNode(const MsgType& msg)
    : msg_(msg)
{
    // Every message is part of the "all" (index 0) label
    labelled_.push_back(true);
    next_.push_back(nullptr);
    prev_.push_back(nullptr);
}

// Complete - Do not alter
LabelList::MsgToken::MsgToken()
 : node_(nullptr), list_(nullptr)
{
}

// To be completed - must adhere to any requirements in the .h file
LabelList::MsgToken::MsgToken(MsgNode* node, LabelList* list)
// Add any initializers here

{
    node_ = node;
    list_ = list;
}

// To be completed - must adhere to any requirements in the .h file
bool LabelList::MsgToken::operator==(const MsgToken& rhs) const
{
    //checking to see if the nodes are the same
    if(rhs.node_ == this->node_) {
        return true;
    }

    else {
        return false;
    }
}

// Complete - Do not alter
bool LabelList::MsgToken::operator!=(const MsgToken& rhs) const
{
    return !operator==(rhs);
}

// To be completed - must adhere to any requirements in the .h file
LabelList::MsgToken::operator bool() const
{
    if(this->node_ == NULL) {
        return false;
    }
    else {
        return true;
    }
}

// Complete - Do not alter
bool LabelList::MsgToken::isValid() const
{
    return this->node_ != nullptr;
}

// Complete - Do not alter
LabelList::MsgToken& LabelList::MsgToken::next(const LabelType& label)
{
    size_t index = this->list_->findLabelIndex(label);
    if( false == this->isLabelMember(index) ) 
    {
        throw std::invalid_argument("Node is not part of the given label");
    }
    this->node_ = this->node_->next_[index];

    // Should return reference to self
    return *this;
}

// Complete - Do not alter
LabelList::MsgToken& LabelList::MsgToken::prev(const LabelType& label)
{
    size_t index = this->list_->findLabelIndex(label);
    if( false == this->isLabelMember(index) ) 
    {
        throw std::invalid_argument("Node is not part of the given label");
    }
    this->node_ = this->node_->prev_[index];

    // Should return reference to self
    return *this;
}

// Complete - Do not alter
bool LabelList::MsgToken::isLabelMember(const LabelType& label) const
{
    size_t index = this->list_->findLabelIndex(label);
    return isLabelMember(index);
}

// Complete - Do not alter
bool LabelList::MsgToken::isLabelMember(size_t index) const
{
    return this->isValid() && 
            index != INVALID_LABEL &&
            index < this->node_->labelled_.size() &&
            this->node_->labelled_[index] ;
}

// To be completed - must adhere to any requirements in the .h file
LabelList::MsgType const & LabelList::MsgToken::msg() const
{
    //if node doesn't exist
    if(this->node_ == nullptr)
    {
        throw std::logic_error("Token references an invalid message");
    }
    else {
        return this->node_->msg_;
    }
}
// To be completed - must adhere to any requirements in the .h file
LabelList::MsgType& LabelList::MsgToken::msg() 
{
    //if node doesn't exist
    if(this->node_ == nullptr)
    {
        throw std::logic_error("Token references an invalid message");
    }
    else {
        return this->node_->msg_;
    }
}

// To be completed - must adhere to any requirements in the .h file
std::ostream& operator<<(std::ostream& ostr, const LabelList::MsgToken& token)
{
    if(token.isValid())
    {
        ostr << token.node_->msg_;
    }
    return ostr;
}

// Static constant 
const LabelList::MsgToken LabelList::end_(nullptr, nullptr);

// To be completed - must adhere to any requirements in the .h file
LabelList::LabelList() 
// Add any initializers here

{
    heads_.push_back(NULL);
    labels_.push_back("all");
    tail_ = NULL;
}

// To be completed - must adhere to any requirements in the .h file
LabelList::~LabelList()
{
    // clear();
}

// To be completed - must adhere to any requirements in the .h file
void LabelList::clear()
{
    // MsgNode* temp = heads_[0];
 
    // //delete each message in all
    // while(temp->next_[0]!= nullptr) {
        
    //     temp = temp->next_[0];
    //     tail_ = nullptr;
    //     delete temp->prev_[0];

    // }
    
    // delete temp;
}

// To be completed - must adhere to any requirements in the .h file
bool LabelList::empty() const
{
    //check to see if all is empty
    if(heads_[0] == NULL) {
        return true;
    }

    else {
        return false;
    }
}

// Complete
size_t LabelList::size() const
{
    MsgNode* n = this->heads_[0];
    size_t cnt = 0;
    while(n != nullptr){
        cnt++;
        n = n->next_[0];
    }
    return cnt;
}

// To be completed - must adhere to any requirements in the .h file
LabelList::MsgToken LabelList::add(const MsgType& msg)
{

    MsgNode* temp_node = new MsgNode(msg); //initalize
    
    //if head is empty
    if(heads_[0] == NULL) {
        heads_[0] = temp_node;
        temp_node->next_[0] = NULL;
        temp_node->prev_[0] = NULL;
        tail_ = heads_[0];
    }
    else{ //if adding onto another node
        temp_node->next_[0] = NULL;
        temp_node->prev_[0] = tail_;
        tail_->next_[0]= temp_node; //og tail next ptr
        tail_ = temp_node; //fix tail
    }

    return MsgToken(temp_node, this);
    
}

// To be completed - must adhere to any requirements in the .h file
void LabelList::remove(const MsgToken& token)
{
    //access the label that isnt put in labels_ yet
    if(!token.isValid()) {
        return;
    }

    MsgNode* node = token.node_;
    for(unsigned int ind = 0; ind <labels_.size();ind++) {
        if(token.node_->labelled_[ind] == true) {
            MsgNode* prev_node = token.node_->prev_[ind];
            MsgNode* next_node = token.node_->next_[ind];

            //if only one in label
            if(node->next_[ind] == NULL && node->prev_[ind] == NULL) {
                heads_[ind] = NULL;
                node->labelled_[ind] = false;
                node->prev_[ind] = NULL;
                node->next_[ind] = NULL;
                continue;
            }

            //if first in the list
            if(node->prev_[ind] == NULL) {
                heads_[ind] = next_node;
                next_node->prev_[ind] = NULL;
                node->labelled_[ind] = false;
                node->prev_[ind] = NULL;
                node->next_[ind] = NULL;
                continue;
            }

            //if last in the label
            if(node->next_[ind] == NULL) {
                prev_node->next_[ind] = NULL;
                node->labelled_[ind] = false;
                node->prev_[ind] = NULL;
                node->next_[ind] = NULL;
                continue;
            }

            //if middle in the label
            if(node->next_[ind] != NULL && node->prev_[ind] != NULL) {
                prev_node->next_[ind] = next_node;
                next_node->prev_[ind] = prev_node;
                node->labelled_[ind] = false;
                node->prev_[ind] = NULL;
                node->next_[ind] = NULL;
                continue;
            }

            
            
        }
        //delete node;
    }


}

// To be completed - must adhere to any requirements in the .h file
void LabelList::label(const MsgToken& token, const LabelType& label)
{
    
    if(!token.isValid())
    {
        throw std::invalid_argument("Bad token in label()");
    }

    MsgNode* node = token.node_;
    bool new_label = false;
    long unsigned int num = 0;

    //check if new label needs to be created
    for(unsigned int i = 0; i < labels_.size(); i++) {
        if (labels_[i] == label) {
            new_label = false;
            break;
        }
        else { 
            new_label = true;
        }
        num++;
    }
    
    //add to a new label
    if(new_label == true) {
        labels_.push_back(label);
     
        //fix labelled_ next_ and prev_ vectors for the MsgNode
        //push_back bool false from last true until you hit the new label
        for(unsigned int i = node->labelled_.size(); i < labels_.size() - 1; i++) {
            node->labelled_.push_back(false);
            node->next_.push_back(nullptr);
            node->prev_.push_back(nullptr);
        }

        node->labelled_.push_back(true);
        node->next_.push_back(nullptr);
        node->prev_.push_back(nullptr);

        //add MsgNode to new label 
        heads_.push_back(node); 
    }

    
    //add to existing label
    if(new_label == false) {
        
        MsgNode* temp = node->prev_[0]; 
        MsgNode* tempnext;
        bool in_label = false; //check to see if temp is in label
    
        //FIX LABELLED_ PREV_ NEXT_ 
        for(unsigned int i = node->labelled_.size(); i < num; i++) {
            node->labelled_.push_back(false);
            node->next_.push_back(nullptr);
            node->prev_.push_back(nullptr);
        }
        node->labelled_.push_back(true);
        node->next_.push_back(nullptr);
        node->prev_.push_back(nullptr);
    
        
        while(temp != NULL) {
            //walk backwards
            //traverse through label to see if temp is in the label

            if(temp->labelled_.size() >= num + 1) { //check vector size of labelled
                if(temp->labelled_[num] == true) { //check boolean
                    in_label = true;
                    break;
                }
                if(temp->labelled_[num] == false) {
                    in_label = false;
                }
            }

            temp = temp->prev_[0]; //is it [0] or [num]
        }
   
        if(!in_label) { //temp not in label
            //insert in beginning
            tempnext = heads_[num];
            heads_[num] = node;
            node->prev_[num] = NULL;
            node->next_[num] = tempnext;
            if(tempnext != NULL) {
                tempnext->prev_[num] = node;  
            }
        }

        
        if(in_label) { //temp in label
        
            //insert between temp and temp next
            if(temp->next_[num] != NULL) {
                tempnext = temp->next_[num];
                node->next_[num] = tempnext;
                temp->next_[num]= node;
                node->prev_[num] = temp;
                if(tempnext != NULL) {
                    tempnext->prev_[num] = node;
                }
            }

            //insert at end
            if(temp->next_[num] == NULL) { 
                temp->next_[num] = node;
                node->next_[num] = NULL;
                node->prev_[num] = temp;
                node->labelled_[num] = true;
            }
            
        }
    }
  

}

// To be completed - must adhere to any requirements in the .h file
void LabelList::unlabel(const MsgToken& token, const LabelType& label)
{
    if(!token.isValid())
    {
        throw std::invalid_argument("Bad token in label()");
    }

    if(findLabelIndex(label) == INVALID_LABEL) {
        throw std::out_of_range("Label does not exist");
    }

    MsgNode* node = token.node_;
    size_t ind = findLabelIndex(label);
    MsgNode* prev_node = token.node_->prev_[ind];
    MsgNode* next_node = token.node_->next_[ind];
    
    //can't unlabel anything from head
    if(label == labels_[0]) {
        return;
    }

    //if only node in the list
    if(node->next_[ind] == NULL && node->prev_[ind] == NULL) {
        heads_[ind] = NULL;
        return;
    }

    //if first in the list
    if(node->prev_[ind] == NULL) {
        heads_[ind] = next_node;
        next_node->prev_[ind] = NULL;
    }

    //if last in the label
    if(node->next_[ind] == NULL) {
        prev_node->next_[ind] = NULL;
    }

    //if middle in the label
    if(node->next_[ind] != NULL && node->prev_[ind] != NULL) {
        prev_node->next_[ind] = next_node;
        next_node->prev_[ind] = prev_node;
    }

    node->labelled_[ind] = false;
    node->prev_[ind] = NULL;
    node->next_[ind] = NULL;

}

// Complete - Do not alter
LabelList::MsgToken LabelList::find(const MsgType& msg, const LabelType& label)
{
    // See if the label exists, will throw if not
    size_t level = this->getLabelIndex(label);
    MsgNode* n = this->heads_[level];
    while(n != nullptr)
    {
        if(n->msg_ == msg)
        {
            return MsgToken(n, this);
        }
        n = n->next_[level];
    }
    return end();
}

// To be completed - must adhere to any requirements in the .h file
LabelList::MsgToken LabelList::find(size_t index, const LabelType& label)
{
    MsgNode* tempnode;

    bool nonex = true;
    for(unsigned int i = 0; i< labels_.size(); i++) {
        if (labels_[i] ==label) {
            nonex = false;
            break;
        }
    }

    if(nonex == true) {
        throw std::out_of_range("non-existent label is provided");
    }

    size_t labelind = getLabelIndex(label);


    
    tempnode = heads_[labelind];

    for(size_t i = 0; i < index; i++) {
         if(tempnode != NULL) {
            tempnode = tempnode->next_[labelind];
        }
        else {
            return end_;
        }
     }

    return MsgToken(tempnode, this);

    
}

// Complete - Do not alter
LabelList::MsgToken const & LabelList::end() const
{
    return end_;
}

// Complete - Do not alter
void LabelList::print(std::ostream& ostr, const LabelType& label, char separator) const
{
    size_t level = this->getLabelIndex(label);
    MsgNode* n = this->heads_[level];
    while(nullptr != n)
    {
        ostr << n->msg_ << separator;
        n = n->next_[level];
    }
    ostr << std::endl;
}

// To be completed - must adhere to any requirements in the .h file
size_t LabelList::findLabelIndex(const LabelType& label) const
{
    for(unsigned int i = 0; i < labels_.size(); i++) {
        if (labels_[i] == label) {
            return i;
        }
    }

    return INVALID_LABEL;

}

// Complete - Do not alter
size_t LabelList::getLabelIndex(const LabelType& label) const
{
    size_t retval = this->findLabelIndex(label);
    if(INVALID_LABEL == retval)
    {
        throw std::out_of_range("Label doesn't exist");
    }
    return retval;
}

