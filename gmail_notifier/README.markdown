### Description
_gmail_notifier_ is a small daemon that will poll gmail every 10 minutes for unread messages,
and display a list of all senders in a [GROWL](http://growl.info/) notification.

### Usage
Start the gmail_notifier daemon with:
    ./gmail_notifier_control start
And stop it with:
    ./gmail_notifier_control stop
Note that you will need a JSON configuration file `~/.gmail`:
    {
      "login":    "user.name",
      "password": "password"
    }

### Libraries
- [daemons](http://daemons.rubyforge.org/): Allows the creaton of daemons and controllers easily
- [ruby-gmail](https://github.com/dcparker/ruby-gmail): GMail manipulation in ruby

### Author
- Aurélien Vallée – <vallee.aurelien@gmail.com>