import { Component, OnInit } from '@angular/core';
import { UserLocationComponent } from '../user-location/user-location.component';
import { UserChangePasswordComponent } from '../user-change-password/user-change-password.component';

@Component({
  selector: 'app-user-security',
  templateUrl: './user-security.component.html',
  styleUrls: ['./user-security.component.css']
})
export class UserSecurityComponent implements OnInit {

  constructor() { }

  ngOnInit() {
  }

}
