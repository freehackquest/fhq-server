import { Component, OnInit } from '@angular/core';
import { SpinnerService } from '../../services/spinner.service';
import { ActivatedRoute } from '@angular/router';

@Component({
  selector: 'app-ctf-wall',
  templateUrl: './ctf-wall.component.html',
  styleUrls: ['./ctf-wall.component.css']
})
export class CtfWallComponent implements OnInit {

  constructor(
    private _spinnerService: SpinnerService,
    private _route: ActivatedRoute,
  ) { }

  ngOnInit(): void {
    this._route.params.subscribe( (params) => {
      this._spinnerService.hide();
    });
  }

}
