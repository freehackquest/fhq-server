import { Component, OnInit, ViewChild, ChangeDetectorRef, ElementRef } from '@angular/core';
import { SpinnerService } from '../../services/spinner.service';
import { Router } from '@angular/router';
import { FhqService } from '../../services/fhq.service';

@Component({
  selector: 'app-scoreboard',
  templateUrl: './scoreboard.component.html',
  styleUrls: ['./scoreboard.component.css']
})
export class ScoreboardComponent implements OnInit {
  errorMessage: string = null;
  dataList: Array<any> = [];

  constructor(
    private _spinnerService: SpinnerService,
    private _cdr: ChangeDetectorRef,
    private _router: Router,
    private _fhq: FhqService,
  ) { }

  ngOnInit() {
    this.loadData();
  }

  loadData() {
    const _data = {
      "page": 0,
      "onpage": 10
    }
    this._spinnerService.show();
    this._fhq.api().scoreboard(_data)
      .done((r: any) => this.successResponse(r))
      .fail((err: any) => this.errorResponse(err));
  }

  successResponse(r: any) {
    console.log(r);
    this._spinnerService.hide();

    this.dataList = []
    r.data.forEach((el: any) => {
      this.dataList.push(el);
    });
    this._cdr.detectChanges();
  }

  errorResponse(err: any) {
    this._spinnerService.hide();
    this.errorMessage = err.error;
    this._cdr.detectChanges();
    console.error(err);
  }
}

/*

fhq.ui.loadScoreboard = function(){

	fhq.ui.showLoading();
	var el = $("#content_page");
	el.html('Loading...');

	var onpage = 5;
	if(fhq.containsPageParam("onpage")){
		onpage = parseInt(fhq.pageParams['onpage'], 10);
	}

	var page = 0;
	if(fhq.containsPageParam("page")){
		page = parseInt(fhq.pageParams['page'], 10);
	}
	
	window.fhq.changeLocationState({'scoreboard':'', 'onpage': onpage, 'page': page});

	var params = {};
	params.onpage = onpage;
	params.page = page;

	fhq.ws.scoreboard(params).done(function(r){
		el.html('<h1>' + fhq.t('Scoreboard') + '</h1>');
		console.log(r);
		for (var k in r.data) {
			
			var arr = [];
			var row = r.data[k];
			var first_user_logo = ''
			for (var k2 in row.users) {
				var u = row.users[k2];
				first_user_logo = u.logo;
				arr.push(fhq.ui.makeUserIcon(u.userid, u.logo, u.nick, u.university));
			}
			
			el.append(''
				+ '<div class="card">'
				+ '	<div class="card-body rating-left-img" id="place' + k + '">'
				+ '		<h1>' + row.place + ' [' + row.rating + ' Points]</h1> '
				+ '		' + arr.join(' ')
				+ '	</div><br>'
				+ '</div><br>'
			);
			
			if(row.users.length == 1)	{
				$('#place' + k).css({'background-image': 'url(' + u.logo + ')'});
			}else{
				$('#place' + k).css({'background-image': 'url(files/users/0.png)'});
			}
		}
		fhq.ui.hideLoading();
	});
}


*/