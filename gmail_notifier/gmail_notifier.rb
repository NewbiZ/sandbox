#!/usr/bin/env ruby

# You need to define your login and password in JSON format in ~/.gmail :
# {
#   "login":    "user.name",
#   "password": "your_password"
# }

require 'ruby-growl'
require 'gmail'

growl = Growl.new "localhost", "ruby-growl", ["growl-gmail-notifier"]
gmail_config = JSON.parse( IO.read("/Users/newbiz/.gmail") )
gmail = Gmail.new gmail_config["login"], gmail_config["password"]

loop do
  # Get some useful informations
  emails_count = gmail.inbox.count(:unread)
  emails_from  = gmail.inbox.emails(:unread).collect { |m| m.from[0].name }
  
  # Notify for new incoming mails
  growl.notify "growl-gmail-notifier", "Incoming mails (#{emails_count})", emails_from.join("\n")
  
  # Wait for 10 minutes
  sleep 600
end

gmail.logout