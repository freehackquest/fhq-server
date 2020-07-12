import { Component, OnInit, Inject, ChangeDetectorRef, Input } from '@angular/core';
import {
  L10N_CONFIG,
  L10nConfig,
  L10N_LOCALE,
  L10nLocale,
  L10nTranslationService
} from "angular-l10n";
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
  schema = this.l10nConfig.schema;

  constructor(
    @Inject(L10N_LOCALE) public _locale: L10nLocale,
    @Inject(L10N_CONFIG) private l10nConfig: L10nConfig,
    public _translationService: L10nTranslationService,
    private _cdr: ChangeDetectorRef,
    private _modalService: NgbModal,
    public _fhq: FhqService,
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
    console.log("lang: ", this._translationService.getLocale().language);
        
    this._fhq.api().bind('server', (data: any) => this.serverInfo(data));

    this.subscription = this._fhq.changedState
      .subscribe(() => this.wsChangedState());
    
    this._spinner.show();
    this._fhq.connectToServer();

    this._translationService.onChange().subscribe({
      next: () => {
        this.menuLangIcon = "assets/img/lang_" + this._translationService.getLocale().language + ".png";
      }
  });

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

  setLocale(locale: L10nLocale): void {
    this._translationService.setLocale(locale);
    // this.menuLangIcon = "assets/img/lang_" + locale.language + ".png";
  }

  openDialogSignIn() {
    const modalRef = this._modalService.open(ModalDialogSignInComponent);
    modalRef.componentInstance.name = 'SignIn';
  }

  serverInfo(data: any) {
    this.serverAppName = data.app;
    this.serverAppVersion = data.version;
    this.libVersion = this._fhq.api().appVersion;
  }
}
