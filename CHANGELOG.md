# fhq-server Cookbook Changelog

This file is used to list changes made in each version of the fhq-server.

# Notice

1. For date of version please use `date -R`
2. Please use current format, because it will be used for ppa/debian changelog

## [0.2.21] (?)

- Code refactoring
- Prepare jobs_pool mechanizm
- Integrated light web-server
- Added web-admin sources

## [0.2.20] (Fri, 08 Mar 2019 00:04:42 +0700)

- Code refactoring
- Improved working with lxd

## [0.2.17] (Fri, 30 Nov 2018 00:18:41 +0700)

- Replaced QCryptographicHas to smallsha1 implementation
- Added notification on users quest passed
- Redesign in EmployLeak for using EmployGame
- Implemented addGame by employ with cache
- Renamed methods id to localId for ModelLeak
- Improved ModelGame
- redesign answers in employ leaks
- Fixed send to all by another thread
- Prepare employ for notifies
- Added to event that game was removed
- Added model notification
- Fixed #111 Game remove - must be cleanup logo
- Added list of server developers
- Removed QCode::AppVersion from code
- Added version and handle required params on cli side
- Added first message from server: version
- Changed folder from files/games to public/games
- Fixed #108 Add employ games with check settings for public/files folder icon games
- Prepare employ_games
- Fixed #103 Game update logo must be work by uuid
- Fixed title -> name #105
- Add help func lxd in cli #94
- Added test for upload logo game
- Added test_games
- Implemented load leaks from database on start server to cache
- Redesign leaks_Add
- Removed unused files
- Moved log to utils_logger
- Redesign class error
- Start redesign leaks
- Minor changes in model_leak

## [0.2.16] (Sat, 07 Jul 2018 03:02:13 +0700)

- Fixed crash
- Added leaks handlers
- Added lxd container handlers
- Added generator python lib cli

## [0.2.15] (Fri, 27 Apr 2018 02:02:57 +0700)

- Added postinst script
