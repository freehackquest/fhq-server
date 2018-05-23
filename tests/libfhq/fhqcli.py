import pytest
import json
import asyncio
import functools
import websockets

class FHQCli:
    __ws = None;
    __url = "ws://localhost:1234/";
    __messageId = 0;
    def __init__(self):
        print("FHQCli Created");

    def setUrl(self, url):
        self.__url = host;

    def getUrl(self):
        return self.__url;

    async def handler(websocket, path, extra_argument):
        print("handler")

    @asyncio.coroutine
    def __tryConnect(self):
        self.__ws = yield from websockets.connect(self.__url);

    def tryConnect(self):
        asyncio.get_event_loop().run_until_complete(self.__tryConnect());
        print("Connected to " + self.__url);
        return True;

    @asyncio.coroutine
    async def __command_send(self, cmd, requestJson):
        self.__messageId = self.__messageId + 1;
        msgId = "m" + str(self.__messageId);
        requestJson["m"] = msgId;
        requestJson["cmd"] = cmd;
        requestText = json.dumps(requestJson)
        print("requestText: " + requestText);
        await self.__ws.send(requestText)

        return msgId;
        
        #responseText = await self.__ws.recv()
        #print("responseText: " + responseText);

    def login(self, email, password):
        requestJson = {}
        requestJson["email"] = email;
        requestJson["password"] = password;
        t = asyncio.get_event_loop().run_until_complete(self.__command_send("login", requestJson));

        print(t);
