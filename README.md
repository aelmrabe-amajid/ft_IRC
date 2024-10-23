
* Join command will be received from client ( Add channel ) this option is available in Limechat as future.  It's ask about Channel name , channel pass , also it's gove some other options like channel topic and channel mode and more .... 

There some case that can be happening here : 
1. Channel doesn't exist, so we will receive Join but we will create channel .  (This must follow some rules )
2. Channel exist and client send join command with correct details. ( this must follow some rules )
3. Channel exist and client provided extra info ( and this info doesn't  have meaning at this case) 

case 1: allowing or rejecting it's depending the validation of creation rules . If rules are valid we will allow else we will reject with error code.  Also here we will check Joining Rules if it's valid or no ? if yes so this request are allowed and new channel will be credited and requester will be the Operator of this channel.  

Case 2: at this case channel exist mean this channel . Mean there is some rules to be checked to ensure that this user can Joining.  Channel private. Channel is invited only ... channel limits and ban list  ... if everthing are ok so the join request will be valid . 


Case 3: at this case the channel exist but you give me some info we don't need for existing channel like specify the mode or topic ... all this info will be used for creation of new channels  not for exited channel so here will return  a error that indicates the error . 


 : JOIN Rules : 

Channel Names Rule : 

•  The Name of channel can start with # or & . This two landing indicate type of channel . In case of multiple server network.  The channel the shared channel will start with & . So because this is not required our channel name will start with # . 
• The channel name length can support 200 char at maximum.  
• There is some forbidden character if channel name like ASCII 7 and spaces and .

Channel Command Syntax Rule : 

• The Join Command must be wrting using this format : 
-  Command: JOIN  
- Parameters: <channel>{,<channel>} [<key>{,<key>}]

This means that we can Join on or more Channel at the same times : here we have our own behavior.  ( if you have more that one channels and 3 valid request and one channel have a error we will handle all valid request and reject not valid case. 

Also this format can define the order of valid request 

Join #mychannel key : single channel and key is password of this channel 

Join #mychannel,#mychannelb key,keyb : case when we have 2 channels key is password for mychannel and keyb is for mychannelb . So it's respect the order. 

Join #mychannel,#mychannelb keyb : this case will use keyb as password for mychannel . (In this example that will be a error of password) the logic of this order it's give us another additional rule : you must start by channels that require key if channel doesn't require password must be put it at last position.  Because keys will be work with index of channels name . Key in index 0 is for channel at the same index. 

JOIN Command Return : 

It's obviously that we have only two general case valid or no valid . Yes that correct but don't forget that this is a global result depending on some sub checkes . That means in case of failure will will returning the corresponding error number.  

• So because the easy return is when everything work as expected and command valid  (bot will send welcome message in channel ) and this message will be broadcast to all client .
• Failure case is depending on several  reasons.  
• There is a case when channel is private so it's respect password.  When you send join command with just one parameter is channel_name or without channel_name that mean NEEDMOREPARAMS, so there's as error (ERR_NEEDMOREPARAMS)

• There is a case when user try to join existing channel . But it's BANED From this channel so return will be 
          ERR_BANNEDFROMCHAN  
• There's a case when user try to join existing channel that require invitation. so in this cas we will return  ERR_INVITEONLYCHAN 
• There's a case when user will send a join command with channel_name and key but this key doesn't match channel key so  ERR_BADCHANNELKEY   
• There's a case when channel have a limits of user that can Join . And our user index will be > then limits  so here we will return ERR_CHANNELISFULL      
• When we try to join a Channel with no existing name this must be in the case where we can't create channel.  (New to be Check this Rule if it's valid) if this rule is exist so in case i can't create and name of channel is not correct we have ERR_NOSUCHCHANNEL
•  We have a case where the Client try to join channel and it's already member in 10 channels so reaching the max of channel  that i can join at the same time so at this case we will return ERR_TOOMANYCHANNELS  

Implementation Plan 


Join Command can be passed by Command directly or using Client[limechat] options we have a options to add channel this will automatic send a Join command . The good things here is this both cases have the same syntax.  
:pekenbower JOIN #mychannel 

1. The first token here is the client nickname or username if the nickname is not giving for this client . 
2. The second token is the command Name 
3. The Command Options 

So here we will start by check the current User it's must be allowed to use the Command in this case Join so i will check the Join requirements for Client 










JOIN REQUIREMENTS FOR CLIENT:
• Client is a client on this server . 
• Client doesn't reach channel limits (10 channel)
• Channel name is exist
• Client is not BANED in target channel 
• Client is not a member in this channel
• Channel exist and it's private 
• Password is given
• Password is wrong 
• Channel name is not exist and i can create channel 
• Channel Name valid 
• Channel details given
• Password are given and following Rules
• Topic are given and following rules 

Run the command Join with options and return . Else return error Number.  


