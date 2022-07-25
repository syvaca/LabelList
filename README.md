# LabelList

This is a project that replicates an email inbox. Explanations to different functions and objects are listed in the labellist.h file and is neccesary to read in order to understand my implementation.

Some key objects that are commonly used are:

A MsgNode, that is the actual message/email. A MsgToken is an object that includes a pointer to its message(MsgNode) and the folder (list) it's placed in. heads_ is a vector containing all the folders messages can be in, and "all" is the first value/folder (this contains all the messages)

Something to not is that all messages within each folder are in a linked list and can only be accessed/altered from walking the list.

I have defined operations to add/remove messages within an email account as well as create folders.


HOW TO RUN THE TESTS

...
