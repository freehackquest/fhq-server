import { Component, OnInit, Inject, ChangeDetectorRef, Input } from '@angular/core';
import { LocaleService, TranslationService, Language } from 'angular-l10n';
import { NgbModal } from '@ng-bootstrap/ng-bootstrap';
import { ModalDialogSignInComponent } from './dialogs/modal-dialog-sign-in/modal-dialog-sign-in.component';
import { FhqService } from './services/fhq.service';
import { Router, NavigationEnd } from '@angular/router';
import { SpinnerService } from './services/spinner.service';

declare var $: any;

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent implements OnInit {
  menuLangIcon: string = '';
  subscription: any;
  serverAppName: string = '';
  serverAppVersion: string = '';
  libVersion: string = '';
  brokenConnection: boolean = false;

  constructor(
    public _locale: LocaleService,
    public _translation: TranslationService,
    private _cdr: ChangeDetectorRef,
    private _modalService: NgbModal,
    private _fhq: FhqService,
    private _router: Router,
    private _spinner: SpinnerService,
  ) {
    //
    this._router.routeReuseStrategy.shouldReuseRoute = function(){
      return false;
    };

    this._router.events.subscribe((evt) => {
      if (evt instanceof NavigationEnd) {
          this._router.navigated = false;
          window.scrollTo(0, 0);
      }
    });
  }

  ngOnInit(): void {
    this.updateLanguage();
    console.log("lang: ", this._locale.getCurrentLanguage());
    this._fhq.api().bind('server', (data: any) => this.serverInfo(data));

    this.subscription = this._fhq.changedState
      .subscribe(() => this.wsChangedState());
    
    this._spinner.show();
    this._fhq.connectToServer();
  }

  ngOnDestroy() {
    this.subscription.unsubscribe();
  }

  wsChangedState() {
    if (this._fhq.connectionState == 'BROKEN') {
      this.brokenConnection = true;
      this._spinner.show();
    }
    if (this._fhq.connectionState == 'OK' && this.brokenConnection === true) {
      this.brokenConnection = false;
      console.log(this._router.url);
      this._router.navigateByUrl(this._router.url);
      this._spinner.hide();
    }
    this._cdr.detectChanges()
  }

  userSignout() {
    this._fhq.logout();
  }

  selectLanguage(language: string): void {
    this._locale.setCurrentLanguage(language);
    this.updateLanguage();
  }

  updateLanguage() {
    const l = this._locale.getCurrentLanguage();
    this.menuLangIcon = "assets/img/lang_" + l + ".png";
    this._cdr.detectChanges();
  }

  openDialogSignIn() {
    const modalRef = this._modalService.open(ModalDialogSignInComponent);
    modalRef.componentInstance.name = 'SignIn';
  }

  openPageNews() {
    this._router.navigate(['/news', 0]);
    // $('.navbar-collapse').collapse('hide');
  }

  serverInfo(data: any) {
    this.serverAppName = data.app;
    this.serverAppVersion = data.version;
    this.libVersion = this._fhq.api().appVersion;
  }
}
