#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import json
import asyncio
import functools
import websockets

async def text_send():
    async with websockets.connect("ws://localhost:1234") as websocket:
        await websocket.send("some")
        print(f"> some")

        greeting = await websocket.recv()
        print(f"< " + greeting)

async def json_send():
    async with websockets.connect("ws://localhost:1234") as websocket:
        await websocket.send("{\"cmd\":\"stop\",\"m\":\"1\"}")
        print(f"> some")

        greeting = await websocket.recv()
        print(f"< " + greeting)

def test_ws_crash():
  print("Test WS Crash")
  asyncio.get_event_loop().run_until_complete(text_send())

def test_ws_json():
  print("Test WS Json")
  asyncio.get_event_loop().run_until_complete(json_send())

test_ws_crash()
test_ws_json()