import json
import asyncio
from libfhq.fhqcli import FHQCli


def test_scoreboard_unquthorized():
  print("Test scoreboard for unauthorized users")

  # asyncio.get_event_loop().run_until_complete(hello())

  # init fhq lib
  fc = FHQCli()
  print("Try connect to " + fc.getUrl());
  fc.tryConnect()
  fc.login("admin","admin");

  
  assert 1 == 2

