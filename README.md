# ft_irc
Internet Relay Chat

## Socket connections
Creation of socket-based connections requires several operations.  First, a socket is created with `socket(2)`.
 Next, a willingness to accept incoming connections and a queue limit for incoming connections are specified with
 `listen()`.  Finally, the connections are accepted with `accept(2)`.  The `listen()` call applies only to sockets of
 type `SOCK\_STREAM`.

## Channels
A channel is a named group of one or more clients. It is created implicitly when the first client joins it, ceases to exist when the last client leavs it. Channel names do not containe any spaces `(' ', 0x20)`, a control G / BELL `('^G', 0x07)`, or a comma `(',', 0x2C)`.;

### Channel types
- regular channel : known to all servers connected to the network (prefix : #)
- local channel : the clinets connected can only see and talk to other clinets on the same server (prefix : &)
- others in [channel types](https://modern.ircdocs.horse/#channel-types)

### Chanops (channel operators)


### user modes
Invisible User Mode : (id letter : '+i') the user will not show up in commandes such as `WHO` and `NAMES`. Some servers hide all channels from the WHOIS reply of an invisible user they do not share 

# Very Helpful pages
[IRC god tutorial](https://ircgod.com/posts/)
[The IRC tutorial](https://datatracker.ietf.org/doc/html/rfc2812)







