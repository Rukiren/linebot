import os
import requests
from flask import Flask, request, abort
from linebot import LineBotApi, WebhookHandler
from linebot.exceptions import InvalidSignatureError
from linebot.models import MessageEvent, TextMessage, TextSendMessage

app = Flask(__name__)

line_bot_api = LineBotApi('')
handler = WebhookHandler('')

openai_api_key = ''
openai_endpoint = ''

@app.route('/callback', methods=['POST'])
def callback():
    signature = request.headers['X-Line-Signature']
    body = request.get_data(as_text=True)
    try:
        handler.handle(body, signature)
    except InvalidSignatureError:
        abort(400)
    return 'OK'

@handler.add(MessageEvent, message=TextMessage)
def handle_message(event):
    text = event.message.text
    print(text)
    headers = {
        'Content-Type': 'application/json',
        'Authorization': f'Bearer {openai_api_key}'
    }
    data = {
        'prompt': text,
        'max_tokens': 60,
        'temperature': 0.5,
        'stop': ['\n']
    }
    response = requests.post(openai_endpoint, headers=headers, json=data)
    response_data = response.json()
    if 'choices' in response_data and len(response_data['choices']) > 0 and 'text' in response_data['choices'][0]:
        text = response_data['choices'][0]['text']
    else:
        text
