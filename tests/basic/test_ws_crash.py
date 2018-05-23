import json
import asyncio
import functools
import websockets
from libfhq.fhqcli import FHQCli

async def text_send():
    fc = FHQCli();
    async with websockets.connect(fc.getUrl()) as websocket:
        await websocket.send("some")
        print(f"> some")

        greeting = await websocket.recv()
        print(f"< " + greeting)

def test_ws_crash():
  print("Test WS Crash")
  asyncio.get_event_loop().run_until_complete(text_send())

