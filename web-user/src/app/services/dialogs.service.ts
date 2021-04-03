import { Injectable } from '@angular/core';
import { ModalSignInDialog } from '../dialogs/modal-signin-dialog/modal-signin-dialog.component';
import { MatDialog } from '@angular/material/dialog';

@Injectable({
  providedIn: 'root'
})
export class DialogsService {

  constructor(
    public _dialog: MatDialog,
  ) { }


  openSignInDialog() {
    const dialogRef = this._dialog.open(ModalSignInDialog, {
      width: '450px',
      data: {}
    });

    dialogRef.afterClosed().subscribe(result => {
      // console.log('The dialog was closed');
      // TODO update states
    });
  }
}
