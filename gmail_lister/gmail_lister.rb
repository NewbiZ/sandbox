#!/usr/bin/env ruby

# You need to define your login and password in JSON format in ~/.gmail :
# {
#   "login":    "user.name",
#   "password": "your_password"
# }

require 'gmail'
require 'json'
require 'yaml'
require 'date'
require 'tmail'

gmail_config = JSON.parse( IO.read("/Users/newbiz/.gmail") )
gmail = Gmail.new gmail_config["login"], gmail_config["password"]

# Get some useful informations
unread_list  = gmail.inbox.emails(:unread, {:on => Date.today})
unread_count = unread_list.length

read_list    = gmail.inbox.emails(:read, {:on => Date.today})
read_count   = read_list.length

puts "Unread (#{unread_count}):"
unread_list.each do |message|
  puts "#{TMail::Unquoter.unquote_and_convert_to(message.from[0].name,'utf-8')} (#{message.from[0].mailbox}@#{message.from[0].host}) - #{TMail::Unquoter.unquote_and_convert_to(message.subject,'utf-8')}"
end
puts ""
puts "Read (today):"
read_list.each do |message|
  puts "#{TMail::Unquoter.unquote_and_convert_to(message.from[0].name,'utf-8')} (#{message.from[0].mailbox}@#{message.from[0].host}) - #{TMail::Unquoter.unquote_and_convert_to(message.subject,'utf-8')}"
end

gmail.logout
