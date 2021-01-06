# fhq-server Cookbook Changelog

All notable changes to fhq-server project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/)
and this project adheres to [Semantic Versioning](http://semver.org/).

## [v0.2.35] - ??



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
