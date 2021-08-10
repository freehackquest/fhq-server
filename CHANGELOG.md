# fhq-server Cookbook Changelog

All notable changes to fhq-server project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/)
and this project adheres to [Semantic Versioning](http://semver.org/).

## [v0.2.42] - 2021-06-11 (2021 June 14)

### Changed

- Small redesign chat page

### Fixed 

- Fixed #358 fix translates in kb

### Security

- Upgrade angular to 12.0.4
- Fixed alert by lgtm

## [v0.2.41] - 2021-05-09 (2021 May 9)

### Added

- Link to https://vk.com/freehackquest

### Changed

- Redesign block-contact-us to material angular

### Fixed 

- Fixed #81 my answers and fixed problem: all answers for any user
- Fixed #360 Add new icon "event_success.png" for server started
- web-admin: Restore dialog signin
- server-api-tests: fixed warnigns by pylint

## [v0.2.40] - 2021-04-11 (2021 Apr 11)

### Added

- New handler `games.info`

### Changed

- Renamed: api.games to api.games_list
- Renamed libfreehackquestclient to freehackquest_lbclient_py
- Method `games_info` allowed to all users.

### Security

- Fixed alerts by lgtm
- npm audit fix
- Fixed alerts by deepcode

## [v0.2.38] - 2021-04-03 (2021 Apr 3)

### Added

- Addded shield 'Server API Coverage'

### Fixed

- Fixed #304 translation 'questHints'

### Changed

- Redesign sign dialog to material
- Updated behaviour for signin dialog
- Replaced subject 'FreeHackQuest 2017' to freehackquest.com
- Redesign test public events to pytest
- Redesign test scoreboard to pytest
- Redesign test_serversettings to pytest

### Security

- Fixed lgtm alerts
- Added lgtm c++ to README.md

## [v0.2.37] - 2021-03-27 (2021 Mar 27)

### Changes

- Redesign docker-compose (moved to contrib/docker_compose_example)
- Moved fhq-server to root of repository
- Moved web-site-admin to web-admin
- Moved user-web-site to web-user

### Security

- Fixed few alerts by lgtm
- Fixed few alerts by deepcode

## [v0.2.36] - 2021-03-15 (2021 Mar 15)

### Changed

- Migrated game tests from custom to pytest
- Fixed export js library
- user-web: 'admin panel' to main menu
- Redesing generator python cli library (spliting by api-groups, use a dot)

### Security

- npm audit fix
- Fixed several alerts by lgtm

## [v0.2.35] - 2021-01-06 (2021 Jan 06)

### Added

- fhq-server: #81 Implemented new method 'quests_user_answer_list'
- Added sea5kg_cpplint.config
- Added to README.md information about valgrind from #101
- Fixed #184 added warning to feedback forms
- Added development information for api-lists

### Changed

- Fixed #337 Rename folder web-admin to admin-web-site
- admin-web-site: redesign quest's pages
- admin-web-site: improved modal dialog
- Fixed #328 Redesing quests_proposal_list params and response
- Renamed WsjcppError to WsjcppJsonRpc20Error
- Redesign README.md and folders  install, misc

### Fixed

- Fixed wordings
- Fixed #276 reimplemented select language to angular mat-select
- Fixed #276: reimplemented navbar for mobiles
- Fixed #332: broken quest / add hint
- Fixed #205: removed user token from logs on login
- Fixed #18: drop column games.rules
- Fixed #72: away from QJson...
- admin-web-site: Fixed dialog delete user
- Fixed Impove export web-js library #330
- Fixed wall
- Fixed #323 Improve documentation for pytest
- Fixed pylint warnings & criticals

### Removed

- Fixed #325 static analizing text moved to another repository wsjcpp-static-analizing-text

### Security

- Bump highlight.js from 10.1.1 to 10.4.0 in /fhq-web-user
- Updated wsjcpp-validators
- Fixed TODO check error database query
- user-web-site: Fixed security alert "webpack-subresource-integrity": ">=1.5.1"
- Fixed #334: admin-web-site user can not remove self

## [v0.2.34] - 2020-10-18 (2020 Oct 18)

### Added

- Fixed #317 print text of quest proposal in a list
- Fixed #315 Add new settings to admin: functional allow_quests_proposals
- searver-api-tests: Splited modes for run tests docker/travis
- Moved information about how to build to README
- Added pylint to travis test (now only for test_leaks.py)
- Added script for angular build production version
- Added .dockerignore file

### Changed

- Fixed generation python library (tested by pylint)
- Some replacemant tabs to spaces in js
- Redesign on admin site showing string settings
- Moved tests_leaks to tests/server-api-tests and redesign to pytest-3
- Moved code-check to tests/code-check
- user-web-site: moved email-validator.service.ts to services/
- Moved doxygen to contrib/
- moved data_travis and script to folder ci/travis

### Fixed

- Fixed errors & warnings by lgtm
- Fixed lgrm error: This property is overwritten by another property in the same object literal.
- Fixed #302: Fix suggestion on registration 'gamil.com' -> fix to 'gmail.com'
- Fixed warnings

### Removed

- web-admin: remmoved unused assets/libs/js/
- web-admin: removed unsed modules && fixed deppcode-critical

### Security

Nope


## [v0.2.33] - 2020-10-16 (2020 Oct 16)

### Added

- Added checks from lgtm.com and deepcode.ai

### Changed

- Updated version of wsjcpp-core module

### Fixed

- Minor style in admin page

## [v0.2.21] - 2020-09-12 (2020 Sep 12)

### Added

- Added web-admin sources
- Prepare jobs_pool mechanizm
- Integrated light web-server

### Changed

- Code refactoring

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
