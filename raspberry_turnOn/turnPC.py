import os, sys
import time
import telepot
import requests
import wiringpi
from time import sleep

def handle(msg):
	chat_id = msg['chat']['id']
	command = msg['text']
	sender = msg['from']['id']
	if sender == int(bot_chatID):
		print ('Got command : %s' % command)
		if command == '/turn' or command =='/turnon':
			wiringpi.digitalWrite(7, 0) #power putton
			sleep(0.4)
			wiringpi.digitalWrite(7, 1) #release power button
			bot.sendMessage(chat_id, "Turning on computer...")
		
		elif command == '/reset' or command == '/reboot':
			wiringpi.digitalWrite(11, 0) #push power putton
			sleep(0.4)
			wiringpi.digitalWrite(11, 1) #release power button
			bot.sendMessage(chat_id, "Rebooting computer...")

		elif command == '/help':
			bot.sendMessage(chat_id, 'Available commands: /turnon /reboot')

		else:
			bot.sendMessage(chat_id, 'Sorry, I do not understand this command')


time.sleep(30)

wiringpi.wiringPiSetupPhys()
wiringpi.pinMode(7, 1)	#reset pin in rele output mode
wiringpi.pinMode(11, 1) #power pin in rele output mode
wiringpi.digitalWrite(7,1)
wiringpi.digitalWrite(11,1)

bot_token = 'YOUR TELEGRAM TOKEN'
bot = telepot.Bot(bot_token)
bot.message_loop(handle)
bot_chatID = 'YOUR CHAT ID'

# Your bot will send you a message in Telegram when it starts
#send_text='https://api.telegram.org/bot' + bot_token + '/sendMessage?chat_id=' + bot_chatID + '&parse_mode=Markdown&text=' + 'Bot available'
#response=requests.get(send_text)
#print(response)

while 1:
	print ('here...')
	time.sleep(10)
