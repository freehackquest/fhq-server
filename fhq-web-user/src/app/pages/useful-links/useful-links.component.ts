import { Component, OnInit, ChangeDetectorRef, ViewChild } from '@angular/core';
import { SpinnerService } from '../../services/spinner.service';
import { FormControl } from '@angular/forms';
import { Subscription } from 'rxjs';
import { Router } from '@angular/router';
import { PageEvent } from '@angular/material';
import { MatTableDataSource } from '@angular/material';
import { FhqService } from '../../services/fhq.service';

export interface UsefulLinkElement {
  id: number;
  link: string;
  description: string;
  rating: number;
  tags: Array<string>;
}

@Component({
  selector: 'app-useful-links',
  templateUrl: './useful-links.component.html',
  styleUrls: ['./useful-links.component.css']
})
export class UsefulLinksComponent implements OnInit {
  public usefullLinksData: UsefulLinkElement[] = [{
    "id": 1,
    "link": "https://freehackquest.com/",
    "description": "CTF training: Online aрхив тасков, платформа для проведения CTF игр",
    "rating": 0,
    "tags": ["ctf", "training", "archive"]
  }, {
    "id": 2,
    "link" : "http://training.hackerdom.ru/",
    "description" :  "CTF training: by from RuCTF organizotrs",
    "rating": 0,
    "tags": ["ctf"]
  }, {
    "id": 3,
    "link": "https://ctfnews.ru/news/",
    "description": "CTF in Russia",
    "rating": 0,
    "tags": ["ctf"]
  }, {
    "id": 4,
    "link": "https://kmb.cybber.ru/",
    "description": "КМБ (Курс молодого бойца) CTF - ресурс, посвященный соревнованиям в области информационной безопасности",
    "rating": 0,
    "tags": ["ctf", "courses"]
  }, {
    "id": 5,
    "link": "https://ctftime.org ",
    "description": "all about ctf (capture the flag) capture the flag, ctf teams, ctf ratings, ctf archive, ctf writeups",
    "rating": 0,
    "tags": ["ctf", "timetable", "rating"]
  }, {
    "id": 6,
    "link": "https://ulearn.me/",
    "description": "Интерактивные онлайн-курсы по программированию",
    "rating": 0,
    "tags": ["courses"]
  }, {
    "id": 7,
    "link": "https://securitylab.ru/",
    "description": "информационный портал по безопасности securitylab.ru, новости, статьи, обзор уязвимостей, вирусов и мнения аналитиков.",
    "rating": 0,
    "tags": ["news", "security"]
  }, {
    "id": 8,
    "link": "http://itsecwiki.org/",
    "description": "informationsecurity wiki",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 9,
    "link": "http://ringzer0team.com/",
    "description": "CTF training/wargames",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 10,
    "link": "https://30.ctf.su",
    "description": "Платформа с тасками task-based/jeopardy",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 11,
    "link": "https://re.mipt.ru",
    "description": "курс 'защита информации'. кафедра радиотехники. мфти.",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 12,
    "link": "https://iso27000.ru ",
    "description": "защита информации, управление информационной безопасностью и рисками",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 13,
    "link": "https://kmb.ufoctf.ru",
    "description": "курс молодого ctf бойца v 1.5",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 14,
    "link": "https://md5hashing.net",
    "description": "ultimate hashing and anonymity toolkit",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 15,
    "link": "https://forum.antichat.ru",
    "description": "antichat - security online community",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 16,
    "link": "https://security.stackexchange",
    "description": "information security stack exchange",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 17,
    "link": "http://hackware.ru",
    "description": "hackware.ru - этичный хакинг и тестирование на проникновение, информационна",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 18,
    "link": "http://yztm.ru",
    "description": "защита информации и эпо",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 19,
    "link": "http://overthewire.org",
    "description": "overthewire: wargames",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 20,
    "link": "http://owasp.org",
    "description": "owasp",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 30,
    "link": "http://root-me.org",
    "description": "CTF training/wargames",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 31,
    "link": "http://canyouhack.it/",
    "description": "CTF training/wargames",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 32,
    "link": "http://www.hackthis.co.uk",
    "description": "CTF training/wargames",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 33,
    "link": "https://hack.me/",
    "description": "THE HOUSE OF THE RISING SANDBOX",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 34,
    "link": "https://xss-game.appspot.com/",
    "description": "Google's XSS vulnerability search game",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 35,
    "link": "https://pwnable.tw/",
    "description": "A newer set of high quality pwnable challenges",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 36,
    "link": "https://www.vulnhub.com/",
    "description": "the essence of the tasks on this resource is to find the vulnerabilities in the issued virtual machine",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 37,
    "link": "https://hackthebox.eu",
    "description": "platform for training and solving problems of hacking services and raising privileges in the OS",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 38,
    "link": "https://ctf365.com/",
    "description": "CTF training/wargames",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 39,
    "link" : "http://smashthestack.org/wargames.html",
    "description" : "Wargames",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 40,
    "link": "http://overthewire.org/wargames/",
    "description": "CTF training/wargames",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 41,
    "link": "https://exploit-exercises.com/",
    "description": "CTF training/wargames",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 42,
    "link": "https://microcorruption.com/login",
    "description": "CTF training/wargames",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 43,
    "link": "http://cryptopals.com/",
    "description": "Matasano Crypto Challenges",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 44,
    "link": "https://legacy.gitbook.com/book/isislab/hack-night/details",
    "description": "Classbook",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 45,
    "link": "http://webcache.googleusercontent.com/search?q=cache:gOdFvGbs7R8J:delimitry.blogspot.com/2014/10/useful-tools-for-ctf.html+&cd=1&hl=ru&ct=clnk",
    "description": "Useful Utilites",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 46,
    "link": "https://trailofbits.github.io/ctf/",
    "description": "CTF Field Guide",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 47,
    "link": "http://pwntools.com",
    "description": "CTF framework used by Gallopsled in every CTF",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 48,
    "link": "https://github.com/Gallopsled/pwntools",
    "description": "CTF framework used by Gallopsled in every CTF",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 49,
    "link" : "https://github.com/zardus/ctf-tools",
    "description" : "Some setup scripts for security research tools.",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 50,
    "link" : "https://apsdehal.in/awesome-ctf/",
    "description" : "A curated list of CTF frameworks, libraries, resources and softwares",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 51,
    "link" : "https://github.com/apsdehal/awesome-ctf",
    "description" : "A curated list of CTF frameworks, libraries, resources and softwares",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 52,
    "link" : "https://github.com/ctfs/resources",
    "description" : "A general collection of information, tools, and tips regarding CTFs and similar security competitions",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 53,
    "link" : "http://ctfs.github.io/resources",
    "description" : "A general collection of information, tools, and tips regarding CTFs and similar security competitions",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 54,
    "link" : "https://github.com/kablaa/CTF-Workshop",
    "description" : "Challenges for Binary Exploitation Workshop",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 55,
    "link" : "https://github.com/bt3gl/My-Gray-Hacker-Resources",
    "description" : "Useful for CTFs, wargames, pentesting. Educational purposes.",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 56,
    "link" : "https://github.com/enddo/awesome-windows-exploitation",
    "description" : "A curated list of awesome Windows Exploitation resources, and shiny things. Inspired by awesom",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 57,
    "link": "https://www.youtube.com/channel/UCS0nOltkP5YXxWggoNVE3Gw/videos?view_as=subscriber",
    "description": "FreeHackQuests write-up's",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 58,
    "link": "https://github.com/ctfs/write-ups-2014",
    "description": "CTF write-up's",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 59,
    "link" : "https://github.com/ctfs/write-ups-2015",
    "description" : "CTF write-up's",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 60,
    "link" : "https://github.com/ctfs/write-ups-2016",
    "description" : "CTF write-up's",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 61,
    "link" : "http://www.pentesteracademy.com/",
    "description" : "Education portal",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 62,
    "link" : "http://www.infosecinstitute.com/",
    "description" : "Education portal",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 63,
    "link" : "http://opensecuritytraining.info/",
    "description" : "Education portal",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 64,
    "link" : "http://securitytrainings.net/",
    "description" : "Education portal",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 65,
    "link" : "http://www.best-masters.com/ranking-master-business-intelligence-knowledge-and-security-management.html",
    "description" : "University",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 66,
    "link" : "http://www.cyberdegrees.org/listings/top-schools",
    "description" : "University",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 67,
    "link" : "https://digitalguardian.com/blog/cybersecurity-higher-education-top-cybersecurity-colleges-and-degrees",
    "description" : "University",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 68,
    "link" : "https://academy.yandex.ru/events/system_administration/kit_2014/",
    "description" : "Courses",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 69,
    "link" : "http://ocw.mit.edu/courses/electrical-engineering-and-computer-science/",
    "description" : "Courses",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 70,
    "link" : "https://expdev-kiuhnm.rhcloud.com/",
    "description" : "Exploit Development Community",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 71,
    "link" : "https://backdoor.sdslabs.co/",
    "description" : "Other",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 72,
    "link" : "http://www.mod-x.co.uk/main.php",
    "description" : "Other",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 73,
    "link" : "http://scanme.nmap.org/",
    "description" : "Other",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 74,
    "link" : "http://www.hackertest.net/",
    "description" : "Other",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 75,
    "link" : "http://net-force.nl/",
    "description" : "Other",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 76,
    "link" : "http://pwnable.kr/",
    "description" : "one of the more popular recent wargamming sets of challenges",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 77,
    "link" : "https://picoctf.com/",
    "description" : "Designed for high school students while the event is usually new every year, it's left online and has a great difficulty progression",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 78,
    "link" : "https://microcorruption.com/login",
    "description" : "one of the best interfaces, a good difficulty curve and introduction to low-level reverse engineering, specifically on an MSP430",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 79,
    "link" : "http://ctflearn.com/",
    "description" : "a new CTF based learning platform with user-contributed challenges",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 80,
    "link" : "https://cherryblog.in/",
    "description" : "",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 81,
    "link" : "http://reversing.kr/",
    "description" : "",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 82,
    "link" : "http://hax.tor.hu/",
    "description" : "",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 83,
    "link" : "https://w3challs.com/",
    "description" : "",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 84,
    "link" : "https://pwn0.com/",
    "description" : "",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 85,
    "link" : "https://io.netgarage.org/",
    "description" : "",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 86,
    "link" : "http://ringzer0team.com/",
    "description" : "",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 87,
    "link" : "http://www.hellboundhackers.org/",
    "description" : "",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 88,
    "link" : "http://www.overthewire.org/wargames/",
    "description" : "",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 89,
    "link" : "http://counterhack.net/Counter_Hack/Challenges.html",
    "description" : "",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 90,
    "link" : "http://www.hackthissite.org/",
    "description" : "",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 91,
    "link" : "http://ctf.komodosec.com",
    "description" : "",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 92,
    "link" : "http://www.wechall.net/sites.php",
    "description" : "excellent list of challenge sites",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 93,
    "link" : "http://ctf.forgottensec.com/wiki/",
    "description" : "good CTF wiki, though focused on CCDC",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 94,
    "link" : "http://repo.shell-storm.org/CTF/",
    "description" : "great archive of CTFs",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 95,
    "link" : "http://demo.testfire.net/",
    "description" : "Webapp Specific",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 96,
    "link" : "http://wocares.com/xsstester.php",
    "description" : "Webapp Specific",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 97,
    "link" : "http://crackme.cenzic.com/",
    "description" : "Webapp Specific",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 98,
    "link" : "http://test.acunetix.com/",
    "description" : "Webapp Specific",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 99,
    "link" : "http://zero.webappsecurity.com/",
    "description" : "Webapp Specific",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 100,
    "link" : "http://computer-forensics.sans.org/community/challenges",
    "description" : "Forensics Specific",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 101,
    "link" : "http://computer-forensics.sans.org/community/challenges",
    "description" : "Forensics Specific",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 102,
    "link" : "http://forensicscontest.com/",
    "description" : "Forensics Specific",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 103,
    "link" : "https://www.praetorian.com/challenges/pwnable/",
    "description" : "Recruiting",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 104,
    "link" : "https://www.root-me.org/",
    "description" : "well done set of challenges free for public use with optional paid commercial versions",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 105,
    "link" : "https://avatao.com",
    "description" : "paid commercial training platform from a CTF team, though with a strong focus on secure app development",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 106,
    "link" : "http://heorot.net/",
    "description" : "",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 107,
    "link" : "http://www.badstore.net/",
    "description" : "Downloadable Offline Games",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 108,
    "link" : "http://www.owasp.org/index.php/Category:OWASP_WebGoat_Project",
    "description" : "Downloadable Offline Games",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 109,
    "link" : "http://www.owasp.org/index.php/Owasp_SiteGenerator",
    "description" : "Downloadable Offline Games",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 110,
    "link" : "http://www.ethicalhack3r.co.uk/damn-vulnerable-web-app/",
    "description" : "Damn Vulnerable Web App",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 111,
    "link" : "http://suif.stanford.edu/~livshits/securibench/",
    "description" : "Stanford SecureBench",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 112,
    "link" : "http://suif.stanford.edu/~livshits/work/securibench-micro/",
    "description" : "Stanford SecureBench Micro",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 113,
    "link" : "http://www.irongeek.com/i.php?page=security/mutillidae-deliberately-vulnerable-php-owasp-top-10",
    "description" : "Downloadable Offline Games",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 114,
    "link" : "https://pentesterlab.com/exercises/",
    "description" : "Virtual Machines",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 115,
    "link" : "http://sourceforge.net/projects/metasploitable/files/Metasploitable2/",
    "description" : "Virtual Machines",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 116,
    "link" : "http://intruded.net/",
    "description" : "Inactive or Gone. Just around for historical sake, or on the off-chance they come back.",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 117,
    "link" : "http://www.foundstone.com/",
    "description" : "Inactive or Gone. Just around for historical sake, or on the off-chance they come back.",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 118,
    "link" : "http://roothack.org/",
    "description" : "Wargames",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 119,
    "link" : "https://ctf.saarland/",
    "description" : "SaarCTF is a classical attack-defense CTF.",
    "rating": 0,
    "tags": [""]
  }, {
    "id": 120,
    "link" : "https://course.ugractf.ru/",
    "description" : "Информационные технологии и их безопасность. Курс для школьников, студентов и всех сочувствующих",
    "rating": 0,
    "tags": ["ctf", "course"]
  }];
  
  pageEvent: PageEvent;
  pageIndex: number = 0;
  pageSize: number = 7;
  length: number = 0;
  pageSizeOptions = [7, 10, 25, 50];
  errorMessage: string = null;

  filteredUsefullLinksData: UsefulLinkElement[] 
  searchValue: String = '';
  searchControl = new FormControl('');
  formCtrlSub: Subscription;

  dataSource = new MatTableDataSource<UsefulLinkElement>();
  displayedColumns: string[] = ['idUsefulLink', 'usefulLinkData'];

  constructor(
    private _spinner: SpinnerService,
    private _cdr: ChangeDetectorRef,
    private _router: Router,
    private _fhq: FhqService,
  ) { }

  ngOnInit() {
    this.updatePage();

    this._spinner.hide();
    this.formCtrlSub = this.searchControl.valueChanges
    .debounceTime(1000)
    .subscribe((newValue) => {
      this.searchValue = newValue
      this.applyFilter();
    });
    this.applyFilter();
    this.dataSource = new MatTableDataSource<UsefulLinkElement>(this.usefullLinksData);
  }

  successUsefulLinksList(r: any) {
    this._spinner.hide();
    // this.usefullLinksData = [];
    for (let i in r.data) {
      let usefulLink = r.data[i];
      this.usefullLinksData.push({
        id: usefulLink['id'],
        link: usefulLink['url'],
        description: usefulLink['description'],
        rating: 0,
        tags: [],
      })
    }
    this.dataSource = new MatTableDataSource<UsefulLinkElement>(this.usefullLinksData);
    this._cdr.detectChanges();
  }

  errorUsefulLinksList(err: any) {
    console.error("errorResponse: ", err);
    this._spinner.hide();
    // this.resultOfChangePassword = err.error;
    // this._cdr.detectChanges();
  }

  updatePage() {
    this._spinner.show();
    this._fhq.api().useful_links_list({})
      .done((r: any) => this.successUsefulLinksList(r))
      .fail((err: any) => this.errorUsefulLinksList(err));
  }

  applyFilter() {
    const _sv = this.searchValue.toUpperCase();
    console.log(_sv);
    this.filteredUsefullLinksData = []
    this.usefullLinksData.forEach((el: any) => {
      var filteredLink = {
        link: el.link,
        description: el.description
      }
      if (el.link.toUpperCase().indexOf(_sv) !== -1
        || el.description.toUpperCase().indexOf(_sv) !== -1) {
          this.filteredUsefullLinksData.push({
            id: el.id,
            link: el.link,
            description: el.description,
            rating: el.rating,
            tags: el.tags
          })
      }
    });
    this.dataSource = new MatTableDataSource<UsefulLinkElement>(this.filteredUsefullLinksData);
  }

  openLink(link) {
    console.log(link);
  }
}
