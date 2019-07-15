import { Component, OnInit, NgZone } from '@angular/core';
import { NgbActiveModal } from '@ng-bootstrap/ng-bootstrap';
import { Router } from '@angular/router';

@Component({
  selector: 'app-block-contact-us',
  templateUrl: './block-contact-us.component.html',
  styleUrls: ['./block-contact-us.component.css']
})
export class BlockContactUsComponent implements OnInit {

  constructor(
    public _activeModal: NgbActiveModal,
    private _zone: NgZone,
    private _router: Router,
  ) { }

  ngOnInit() {
  }

  openDonatePage() {
    this._activeModal.dismiss('');
    this._zone.run(() => this._router.navigate(['/donate'])).then();
  }

}
