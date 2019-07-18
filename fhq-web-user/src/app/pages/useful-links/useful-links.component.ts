import { Component, OnInit } from '@angular/core';
import { SpinnerService } from '../../services/spinner.service';

@Component({
  selector: 'app-useful-links',
  templateUrl: './useful-links.component.html',
  styleUrls: ['./useful-links.component.css']
})
export class UsefulLinksComponent implements OnInit {
  private dataList: Array<any> = [{
    "link": "https://freehackquest.com/",
    "description": "CTF training: Online aрхив тасков, платформа для проведения CTF игр",
  }, {
    "link" : "http://training.hackerdom.ru/",
    "description" :  "CTF training: by from RuCTF organizotrs",
  }, {
    "link": "https://ctfnews.ru/news/",
    "description": "CTF in Russia",
  }, {
    "link": "https://ctftime.org ",
    "description": "all about ctf (capture the flag) capture the flag, ctf teams, ctf ratings, ctf archive, ctf writeups",
  }, {
    "link": "https://ulearn.me/",
    "description": "Интерактивные онлайн-курсы по программированию",
  }, {
    "link": "https://securitylab.ru/",
    "description": "информационный портал по безопасности securitylab.ru, новости, статьи, обзор уязвимостей, вирусов и мнения аналитиков.",
  }, {
    "link": "http://itsecwiki.org/",
    "description": "informationsecurity wiki",
  }, {
    "link": "http://ringzer0team.com/",
    "description": "CTF training/wargames",
  }, {
    "link": "https://re.mipt.ru",
    "description": "курс 'защита информации'. кафедра радиотехники. мфти.",
  }, {
    "link": "https://iso27000.ru ",
    "description": "защита информации, управление информационной безопасностью и рисками",
  }, {
    "link": "https://kmb.ufoctf.ru",
    "description": "курс молодого ctf бойца v 1.5",
  }, {
    "link": "https://md5hashing.net",
    "description": "ultimate hashing and anonymity toolkit",
  }, {
    "link": "https://forum.antichat.ru",
    "description": "antichat - security online community",
  }, {
    "link": "https://security.stackexchange",
    "description": "information security stack exchange",
  }, {
    "link": "http://hackware.ru",
    "description": "hackware.ru - этичный хакинг и тестирование на проникновение, информационна",
  }, {
    "link": "http://yztm.ru",
    "description": "защита информации и эпо",
  }, {
    "link": "http://overthewire.org",
    "description": "overthewire: wargames",
  }, {
    "link": "http://owasp.org",
    "description": "owasp",
  }, {
    "link": "http://root-me.org",
    "description": "CTF training/wargames",
  }, {
    "link": "http://canyouhack.it/",
    "description": "CTF training/wargames",
  }, {
    "link": "http://www.hackthis.co.uk",
    "description": "CTF training/wargames",
  }, {
    "link": "https://hack.me/",
    "description": "THE HOUSE OF THE RISING SANDBOX",
  }, {
    "link": "https://xss-game.appspot.com/",
    "description": "Google's XSS vulnerability search game",
  }, {
    "link": "https://pwnable.tw/",
    "description": "A newer set of high quality pwnable challenges",
  }, {
    "link": "https://www.vulnhub.com/",
    "description": "the essence of the tasks on this resource is to find the vulnerabilities in the issued virtual machine",
  }, {
    "link": "https://hackthebox.eu",
    "description": "platform for training and solving problems of hacking services and raising privileges in the OS",
  }, {
    "link": "https://ctf365.com/",
    "description": "CTF training/wargames",
  }, {
    "link" : "http://smashthestack.org/wargames.html",
    "description" : "Wargames",
  }, {
    "link": "http://overthewire.org/wargames/",
    "description": "CTF training/wargames",
  }, {
    "link": "https://exploit-exercises.com/",
    "description": "CTF training/wargames",
  }, {
    "link": "https://microcorruption.com/login",
    "description": "CTF training/wargames",
  }, {
    "link": "http://cryptopals.com/",
    "description": "Matasano Crypto Challenges",
  }, {
    "link": "https://legacy.gitbook.com/book/isislab/hack-night/details",
    "description": "Classbook",
  }, {
    "link": "http://webcache.googleusercontent.com/search?q=cache:gOdFvGbs7R8J:delimitry.blogspot.com/2014/10/useful-tools-for-ctf.html+&cd=1&hl=ru&ct=clnk",
    "description": "Useful Utilites",
  }, {
    "link": "https://trailofbits.github.io/ctf/",
    "description": "CTF Field Guide",
  }, {
    "link": "http://pwntools.com",
    "description": "CTF framework used by Gallopsled in every CTF",
  }, {
    "link": "https://github.com/Gallopsled/pwntools",
    "description": "CTF framework used by Gallopsled in every CTF",
  }, {
    "link" : "https://github.com/zardus/ctf-tools",
    "description" : "Some setup scripts for security research tools.",
  }, {
    "link" : "https://apsdehal.in/awesome-ctf/",
    "description" : "A curated list of CTF frameworks, libraries, resources and softwares",
  }, {
    "link" : "https://github.com/apsdehal/awesome-ctf",
    "description" : "A curated list of CTF frameworks, libraries, resources and softwares",
  }, {
    "link" : "https://github.com/ctfs/resources",
    "description" : "A general collection of information, tools, and tips regarding CTFs and similar security competitions",
  }, {
    "link" : "http://ctfs.github.io/resources",
    "description" : "A general collection of information, tools, and tips regarding CTFs and similar security competitions",
  }, {
    "link" : "https://github.com/kablaa/CTF-Workshop",
    "description" : "Challenges for Binary Exploitation Workshop",
  }, {
    "link" : "https://github.com/bt3gl/My-Gray-Hacker-Resources",
    "description" : "Useful for CTFs, wargames, pentesting. Educational purposes.",
  }, {
    "link" : "https://github.com/enddo/awesome-windows-exploitation",
    "description" : "A curated list of awesome Windows Exploitation resources, and shiny things. Inspired by awesom",
  }, {
    "link": "https://www.youtube.com/channel/UCS0nOltkP5YXxWggoNVE3Gw/videos?view_as=subscriber",
    "description": "FreeHackQuests write-up's",
  }, {
    "link": "https://github.com/ctfs/write-ups-2014",
    "description": "CTF write-up's",
  }, {
    "link" : "https://github.com/ctfs/write-ups-2015",
    "description" : "CTF write-up's",
  }, {
    "link" : "https://github.com/ctfs/write-ups-2016",
    "description" : "CTF write-up's",
  }, {
    "link" : "http://www.pentesteracademy.com/",
    "description" : "Education portal",
  }, {
    "link" : "http://www.infosecinstitute.com/",
    "description" : "Education portal",
  }, {
    "link" : "http://opensecuritytraining.info/",
    "description" : "Education portal",
  }, {
    "link" : "http://securitytrainings.net/",
    "description" : "Education portal",
  }, {
    "link" : "http://www.best-masters.com/ranking-master-business-intelligence-knowledge-and-security-management.html",
    "description" : "University",
  }, {
    "link" : "http://www.cyberdegrees.org/listings/top-schools",
    "description" : "University",
  }, {
    "link" : "https://digitalguardian.com/blog/cybersecurity-higher-education-top-cybersecurity-colleges-and-degrees",
    "description" : "University",
  }, {
    "link" : "https://academy.yandex.ru/events/system_administration/kit_2014/",
    "description" : "Courses",
  }, {
    "link" : "http://ocw.mit.edu/courses/electrical-engineering-and-computer-science/",
    "description" : "Courses",
  }, {
    "link" : "https://expdev-kiuhnm.rhcloud.com/",
    "description" : "Exploit Development Community",
  }, {
    "link" : "https://backdoor.sdslabs.co/",
    "description" : "Other",
  }, {
    "link" : "http://www.mod-x.co.uk/main.php",
    "description" : "Other",
  }, {
    "link" : "http://scanme.nmap.org/",
    "description" : "Other",
  }, {
    "link" : "http://www.hackertest.net/",
    "description" : "Other",
  }, {
    "link" : "http://net-force.nl/",
    "description" : "Other",
  }, {
    "link" : "http://pwnable.kr/",
    "description" : "one of the more popular recent wargamming sets of challenges",
  }, {
    "link" : "https://picoctf.com/",
    "description" : "Designed for high school students while the event is usually new every year, it's left online and has a great difficulty progression",
  }, {
    "link" : "https://microcorruption.com/login",
    "description" : "one of the best interfaces, a good difficulty curve and introduction to low-level reverse engineering, specifically on an MSP430",
  }, {
    "link" : "http://ctflearn.com/",
    "description" : "a new CTF based learning platform with user-contributed challenges",
  }, {
    "link" : "https://cherryblog.in/",
    "description" : "",
  }, {
    "link" : "http://reversing.kr/",
    "description" : "",
  }, {
    "link" : "http://hax.tor.hu/",
    "description" : "",
  }, {
    "link" : "https://w3challs.com/",
    "description" : "",
  }, {
    "link" : "https://pwn0.com/",
    "description" : "",
  }, {
    "link" : "https://io.netgarage.org/",
    "description" : "",
  }, {
    "link" : "http://ringzer0team.com/",
    "description" : "",
  }, {
    "link" : "http://www.hellboundhackers.org/",
    "description" : "",
  }, {
    "link" : "http://www.overthewire.org/wargames/",
    "description" : "",
  }, {
    "link" : "http://counterhack.net/Counter_Hack/Challenges.html",
    "description" : "",
  }, {
    "link" : "http://www.hackthissite.org/",
    "description" : "",
  }, {
    "link" : "http://ctf.komodosec.com",
    "description" : "",
  }, {
    "link" : "http://www.wechall.net/sites.php",
    "description" : "excellent list of challenge sites",
  }, {
    "link" : "http://ctf.forgottensec.com/wiki/",
    "description" : "good CTF wiki, though focused on CCDC",
  }, {
    "link" : "http://repo.shell-storm.org/CTF/",
    "description" : "great archive of CTFs",
  }, {
    "link" : "http://demo.testfire.net/",
    "description" : "Webapp Specific",
  }, {
    "link" : "http://wocares.com/xsstester.php",
    "description" : "Webapp Specific",
  }, {
    "link" : "http://crackme.cenzic.com/",
    "description" : "Webapp Specific",
  }, {
    "link" : "http://test.acunetix.com/",
    "description" : "Webapp Specific",
  }, {
    "link" : "http://zero.webappsecurity.com/",
    "description" : "Webapp Specific",
  }, {
    "link" : "http://computer-forensics.sans.org/community/challenges",
    "description" : "Forensics Specific",
  }, {
    "link" : "http://computer-forensics.sans.org/community/challenges",
    "description" : "Forensics Specific",
  }, {
    "link" : "http://forensicscontest.com/",
    "description" : "Forensics Specific",
  }, {
    "link" : "https://www.praetorian.com/challenges/pwnable/",
    "description" : "Recruiting",
  }, {
    "link" : "https://www.root-me.org/",
    "description" : "well done set of challenges free for public use with optional paid commercial versions",
  }, {
    "link" : "https://avatao.com",
    "description" : "paid commercial training platform from a CTF team, though with a strong focus on secure app development",
  }, {
    "link" : "http://heorot.net/",
    "description" : "",
  }, {
    "link" : "http://www.badstore.net/",
    "description" : "Downloadable Offline Games",
  }, {
    "link" : "http://www.owasp.org/index.php/Category:OWASP_WebGoat_Project",
    "description" : "Downloadable Offline Games",
  }, {
    "link" : "http://www.owasp.org/index.php/Owasp_SiteGenerator",
    "description" : "Downloadable Offline Games",
  }, {
    "link" : "http://www.ethicalhack3r.co.uk/damn-vulnerable-web-app/",
    "description" : "Damn Vulnerable Web App",
  }, {
    "link" : "http://suif.stanford.edu/~livshits/securibench/",
    "description" : "Stanford SecureBench",
  }, {
    "link" : "http://suif.stanford.edu/~livshits/work/securibench-micro/",
    "description" : "Stanford SecureBench Micro",
  }, {
    "link" : "http://www.irongeek.com/i.php?page=security/mutillidae-deliberately-vulnerable-php-owasp-top-10",
    "description" : "Downloadable Offline Games",
  }, {
    "link" : "https://pentesterlab.com/exercises/",
    "description" : "Virtual Machines",
  }, {
    "link" : "http://sourceforge.net/projects/metasploitable/files/Metasploitable2/",
    "description" : "Virtual Machines",
  }, {
    "link" : "http://intruded.net/",
    "description" : "Inactive or Gone. Just around for historical sake, or on the off-chance they come back.",
  }, {
    "link" : "http://www.foundstone.com/",
    "description" : "Inactive or Gone. Just around for historical sake, or on the off-chance they come back.",
  }, {
    "link" : "http://roothack.org/",
    "description" : "Wargames",
  }];
  
  constructor(
    private _spinner: SpinnerService,
  ) { }

  ngOnInit() {
    this._spinner.hide();
  }

}
