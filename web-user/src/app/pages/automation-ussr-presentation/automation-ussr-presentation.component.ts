import { Component, OnInit } from '@angular/core';
import { SpinnerService } from '../../services/spinner.service';

@Component({
  selector: 'app-automation-ussr-presentation',
  templateUrl: './automation-ussr-presentation.component.html',
  styleUrls: ['./automation-ussr-presentation.component.css']
})
export class AutomationUssrPresentationComponent implements OnInit {

  constructor(
    private _spinner: SpinnerService,
  ) { }

  ngOnInit() {
    this._spinner.hide();
  }

}
