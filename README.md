Messgae delivery system to mimic Instagram notification systems:

By reading jsonl files from command line, it is able to determine the notification settings of individuals and send approrpiate messages to that user given an user id

Some notifications that could be recieved are:
1) message request notifications
2) liked post notification
3) user followed notification
4) user tagged notification
5) user comment notifications

If more than 3 of the same notifications are sent in a row, it will be aggregated together. 
The notifications are sent in order of latest notification to newest
