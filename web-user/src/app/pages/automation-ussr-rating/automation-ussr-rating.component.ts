import { Component, OnInit, ChangeDetectorRef } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { SpinnerService } from '../../services/spinner.service';

@Component({
  selector: 'app-automation-ussr-rating',
  templateUrl: './automation-ussr-rating.component.html',
  styleUrls: ['./automation-ussr-rating.component.css']
})
export class AutomationUssrRatingComponent implements OnInit {
  ratingList: Array<any> = [];
  ratingPogon: Array<any> = [
    { s: -100000, img: 'automation_ussr_00-shtrafnic.png', 			title: 'Штрафник' },
    { s: 0,       img: 'automation_ussr_01-ryadovoi.png', 			title: 'Рядовой' },
    { s: 25,      img: 'automation_ussr_02-efreitor.png', 			title: 'Ефрейтор' },
    { s: 50,      img: 'automation_ussr_03-ml_serjant.png', 			title: 'Младший сержант' },
    { s: 100,     img: 'automation_ussr_04_serjant.png',				title: 'Сержант' },
    { s: 150,     img: 'automation_ussr_05-st_serjant.png', 			title: 'Старший сержант' },
    { s: 300,     img: 'automation_ussr_06-starshina.png', 			title: 'Старшина' },
    { s: 400,     img: 'automation_ussr_07-praporschik.png', 			title: 'Прапорщик' },
    { s: 500,     img: 'automation_ussr_08-st_praporschik.png', 		title: 'Старший прапорщик' },
    { s: 1000,    img: 'automation_ussr_09-ml_leitinant.png', 		title: 'Младший лейтинант' },
    { s: 1500,    img: 'automation_ussr_10-leitinant.png', 			title: 'Лейтинант' },
    { s: 2000,    img: 'automation_ussr_11-st_leitinant.png', 		title: 'Старший лейтинант' },
    { s: 4000,    img: 'automation_ussr_12-kapitan.png', 				title: 'Капитан' },
    { s: 5000,    img: 'automation_ussr_13-major.png', 				title: 'Майор' },
    { s: 6000,    img: 'automation_ussr_14-podpolkovnik.png', 		title: 'Подполковник' },
    { s: 8000,    img: 'automation_ussr_15-polkovnik.png', 			title: 'Полковник' },
    { s: 10000,   img: 'automation_ussr_16-general_major.png', 		title: 'Генерал-майор' },
    { s: 15000,   img: 'automation_ussr_17-general_leitinant.png', 	title: 'Генерал-лейтинант' },
    { s: 20000,   img: 'automation_ussr_18-general_polkovnik.png', 	title: 'Генерал-полковник' },
    { s: 25000,   img: 'automation_ussr_19-general_armii.png', 		title: 'Генерал Армии' },
    { s: 50000,   img: 'automation_ussr_20-marshal.png', 				title: 'Маршал' },
    { s: 100000,  img: 'automation_ussr_21-generalissimus.png', 		title: 'Генералиссимус' },
  ];automation_ussr_

  constructor(
    private _http: HttpClient,
    private _cdr: ChangeDetectorRef,
    private _spinner: SpinnerService,
  ) { }

  ngOnInit() {
    this._http.get("https://freehackquest.com/automation-ussr/api/v1/rating/").subscribe(
      (data: any) => {
        this.ratingList = [];
        data.rating.forEach((row: any) => {
          this.ratingList.push(row);
        });
        this._cdr.detectChanges();
      },
      (err: any) => {
        console.error(err);
        // this.errorResultOfSendFlag = err.error.text
      }
    );
    this._spinner.hide();
  }

  findPogon(score: number) {
    let p = {};
    this.ratingPogon.forEach(row => {
      if (score >= row.s) {
        p = row;
      }
    })
    return p;
  }

}
