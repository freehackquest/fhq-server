import { Component, OnInit, ChangeDetectorRef, ViewChild, ElementRef } from '@angular/core';
import { FhqService } from '../../services/fhq.service';
import { SpinnerService } from '../../services/spinner.service';
import { NgbModal } from '@ng-bootstrap/ng-bootstrap';
import { ModalDialogSignInComponent } from '../../dialogs/modal-dialog-sign-in/modal-dialog-sign-in.component';
import { MatTableDataSource } from '@angular/material';


export interface UserFavoriteLinkElement {
  id: number;
  link: string;
  description: string;
  userFavorites: number;
  favorite: boolean;
  tags: Array<string>;
}

@Component({
  selector: 'app-user-favorites-useful-links',
  templateUrl: './user-favorites-useful-links.component.html',
  styleUrls: ['./user-favorites-useful-links.component.css']
})
export class UserFavoritesUsefulLinksComponent implements OnInit {
  userId: number = -1;
  resultOfUserSkills: string = null;
  subscription: any;
  dataSource = new MatTableDataSource<UserFavoriteLinkElement>();

  userFavoriteLinks: UserFavoriteLinkElement[] = [];
  displayedColumns: string[] = ['idFavoriteLinks', 'listOfFavoriteLinks'];

  constructor(
    private _cdr: ChangeDetectorRef,
    private _fhq: FhqService,
    private _spinnerService: SpinnerService,
    private _modalService: NgbModal,
  ) {

  }

  ngOnInit() {
    this.updatePage();
    this.subscription = this._fhq.changedState
      .subscribe(() => this.updatePage());
  }
  
  updatePage() {
    if (this._fhq.isAuthorized) {
      this.userId = parseInt(this._fhq.userdata.id, 10);
      this.loadUserFavorites();
    } else {
      this.userId = 0;
      this.userFavoriteLinks = [];
    }
    this._spinnerService.hide();
  }

  loadUserFavorites() {
    this._fhq.api().useful_links_user_favorite_list({})
      .done((r: any) => this.successUserFavorites(r))
      .fail((err: any) => this.errorUserFavorites(err));
  }

  successUserFavorites(r: any) {
    console.log(r)
    this.userFavoriteLinks = [];
    for (let ul in r.data) {
      console.log(ul)
      this.userFavoriteLinks.push({
        id: r.data[ul]['id'],
        link: r.data[ul]['url'],
        description: r.data[ul]['description'],
        userFavorites: r.data[ul]['user_favorites'],
        favorite: r.data[ul]['favorite'],
        tags: [],
      })
    }
    this.dataSource = new MatTableDataSource<UserFavoriteLinkElement>(this.userFavoriteLinks);
    this._cdr.detectChanges();
    this._spinnerService.hide();
  }

  errorUserFavorites(err: any) {
    console.error("errorUserFavorites: ", err);
  }

  successUsefulLinksUserUnfavorite(r: any) {
    this.updatePage()
  }

  errorUsefulLinksUserUnfavorite(err: any) {
    console.error("errorResponse: ", err);
    this._spinnerService.hide();
    // this.resultOfChangePassword = err.error;
    // this._cdr.detectChanges();
  }

  removeFromFavorite(id: number) {
    console.log("removeFromFavorite", id );
    this._spinnerService.show();
    this._fhq.api().useful_links_user_unfavorite({
      "useful_link_id": id
    })
      .done((r: any) => this.successUsefulLinksUserUnfavorite(r))
      .fail((err: any) => this.errorUsefulLinksUserUnfavorite(err));
  }

  openDialogSignIn() {
    const modalRef = this._modalService.open(ModalDialogSignInComponent);
    modalRef.componentInstance.name = 'SignIn';
  }
}
