import { BrowserModule } from '@angular/platform-browser';
import { NgModule, APP_INITIALIZER } from '@angular/core';
import { MatBadgeModule } from '@angular/material/badge';
import { MatButtonModule } from '@angular/material/button';
import { MatCardModule } from '@angular/material/card';
import { MatChipsModule } from '@angular/material/chips';
import { MatRippleModule } from '@angular/material/core';
import { MatDividerModule } from '@angular/material/divider';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatIconModule } from '@angular/material/icon';
import { MatInputModule } from '@angular/material/input';
import { MatListModule } from '@angular/material/list';
import { MatPaginatorModule } from '@angular/material/paginator';
import { MatProgressBarModule } from '@angular/material/progress-bar';
import { MatSidenavModule } from '@angular/material/sidenav';
import { MatSnackBarModule } from '@angular/material/snack-bar';
import { MatTableModule } from '@angular/material/table';

// import {MatSidenavModule} from '@angular/material/sidenav';

import { HttpClientModule } from '@angular/common/http';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';

import { NgbModule, NgbModal } from '@ng-bootstrap/ng-bootstrap';
import { APP_BASE_HREF } from '@angular/common';
import { NgbActiveModal } from '@ng-bootstrap/ng-bootstrap';

import { L10nTranslationModule, L10nIntlModule, L10nValidationModule, L10nRoutingModule, L10nLoader } from 'angular-l10n';
import { l10nConfig, initL10n, AppStorage, HttpTranslationLoader, LocaleValidation } from './l10n-config';

import { PageNotFoundComponent } from './pages/page-not-found/page-not-found.component';
import { SpinnerComponent } from './spinner/spinner.component';
import { OverlayModule } from '@angular/cdk/overlay';
import { ServerApiComponent } from './pages/server-api/server-api.component';
import { MapActivityComponent } from './pages/map-activity/map-activity.component';
import { NewsComponent } from './pages/news/news.component';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { ChatComponent } from './pages/chat/chat.component';
import { AutomationUssrComponent } from './pages/automation-ussr/automation-ussr.component';
import { AutomationUssrPresentationComponent } from './pages/automation-ussr-presentation/automation-ussr-presentation.component';
import { AutomationUssrRatingComponent } from './pages/automation-ussr-rating/automation-ussr-rating.component';
import { RegistrationComponent } from './pages/registration/registration.component';
import { ResetPasswordComponent } from './pages/reset-password/reset-password.component';
import { DonateComponent } from './pages/donate/donate.component';
import { UsefulLinksComponent } from './pages/useful-links/useful-links.component';
import { GamesComponent } from './pages/games/games.component';
import { FeedbackComponent } from './pages/feedback/feedback.component';
import { ConfidencialComponent } from './pages/confidencial/confidencial.component';
import { KnowledgeBaseComponent } from './pages/knowledge-base/knowledge-base.component';
import { ModalDialogSignInComponent } from './dialogs/modal-dialog-sign-in/modal-dialog-sign-in.component';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { UserProfileComponent } from './pages/user-profile/user-profile.component';
import { UserProfileMenuComponent } from './pages/user-profile-menu/user-profile-menu.component';
import { AboutComponent } from './pages/about/about.component';
import { QuestProposalComponent } from './pages/quest-proposal/quest-proposal.component';
import { LMarkdownEditorModule } from 'ngx-markdown-editor';
import { AceModule } from 'ngx-ace-wrapper';
import { ACE_CONFIG } from 'ngx-ace-wrapper';
import { AceConfigInterface } from 'ngx-ace-wrapper';
import { ScoreboardComponent } from './pages/scoreboard/scoreboard.component';
import { QuestsComponent } from './pages/quests/quests.component';
import { QuestsBySubjectComponent } from './pages/quests-by-subject/quests-by-subject.component';
import { UserSecurityComponent } from './pages/user-security/user-security.component';
import { QuestComponent } from './pages/quest/quest.component';
import { ModalDialogQuestFeedbackComponent } from './dialogs/modal-dialog-quest-feedback/modal-dialog-quest-feedback.component';
import { BlockContactUsComponent } from './block-contact-us/block-contact-us.component';
import { UserSkillsComponent } from './pages/user-skills/user-skills.component';
import { WelcomeComponent } from './pages/welcome/welcome.component';
import { NgxMdModule } from 'ngx-md';
import { FhqJuryAdComponent } from './pages/fhq-jury-ad/fhq-jury-ad.component';
import { SimplemdeModule } from 'ng2-simplemde';
import { UserTokensComponent } from './pages/user-tokens/user-tokens.component';
import { UserFavoritesUsefulLinksComponent } from './pages/user-favorites-useful-links/user-favorites-useful-links.component'
import { HighlightModule, HIGHLIGHT_OPTIONS } from 'ngx-highlightjs';
import { UsefulLinkViewComponent } from './pages/useful-link-view/useful-link-view.component';

export function getHighlightLanguages() {
  return {
    typescript: () => import('highlight.js/lib/languages/typescript'),
    css: () => import('highlight.js/lib/languages/css'),
    xml: () => import('highlight.js/lib/languages/xml')
  };
}

const DEFAULT_ACE_CONFIG: AceConfigInterface = {
};

@NgModule({
  declarations: [
    AppComponent,
    PageNotFoundComponent,
    SpinnerComponent,
    ServerApiComponent,
    MapActivityComponent,
    NewsComponent,
    ChatComponent,
    AutomationUssrComponent,
    AutomationUssrPresentationComponent,
    AutomationUssrRatingComponent,
    RegistrationComponent,
    ResetPasswordComponent,
    DonateComponent,
    UsefulLinksComponent,
    GamesComponent,
    FeedbackComponent,
    ConfidencialComponent,
    KnowledgeBaseComponent,
    ModalDialogSignInComponent,
    UserProfileComponent,
    AboutComponent,
    QuestProposalComponent,
    ScoreboardComponent,
    QuestsComponent,
    QuestsBySubjectComponent,
    UserProfileMenuComponent,
    UserSecurityComponent,
    QuestComponent,
    ModalDialogQuestFeedbackComponent,
    BlockContactUsComponent,
    UserSkillsComponent,
    WelcomeComponent,
    FhqJuryAdComponent,
    UserTokensComponent,
    UserFavoritesUsefulLinksComponent,
    UsefulLinkViewComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    BrowserModule,
    NgbModule,
    HttpClientModule,
    L10nTranslationModule.forRoot(
      l10nConfig,
      {
          storage: AppStorage,
          translationLoader: HttpTranslationLoader
      }
    ),
    L10nIntlModule,
    L10nValidationModule.forRoot({ validation: LocaleValidation }),
    L10nRoutingModule.forRoot(),
    AppRoutingModule,
    OverlayModule,
    FormsModule,
    AceModule,
    LMarkdownEditorModule,
    ReactiveFormsModule,
    BrowserAnimationsModule,
    NgxMdModule.forRoot(),
    SimplemdeModule.forRoot(),
    HighlightModule,
    MatButtonModule,
    MatFormFieldModule,
    MatInputModule,
    MatIconModule,
    MatTableModule,
    MatPaginatorModule,
    MatCardModule,
    MatDividerModule,
    MatListModule,
    MatSidenavModule,
    MatProgressBarModule,
    MatSnackBarModule,
    MatBadgeModule,
    MatChipsModule,
    MatRippleModule
  ],
  exports: [
    MatButtonModule,
    MatFormFieldModule,
    MatInputModule,
    MatIconModule,
    MatTableModule,
    MatPaginatorModule,
    MatCardModule,
    MatDividerModule,
    MatListModule,
    MatSidenavModule,
    MatProgressBarModule,
    MatSnackBarModule,
    MatRippleModule
  ],
  providers: [
    {
      provide: APP_BASE_HREF,
      useValue: '/'
    }, {
      provide: ACE_CONFIG,
      useValue: DEFAULT_ACE_CONFIG
    }, {
      provide: APP_INITIALIZER,
      useFactory: initL10n,
      deps: [L10nLoader],
      multi: true
    }, {
      provide: HIGHLIGHT_OPTIONS,
      useValue: {
        languages: getHighlightLanguages()
      }
    },
    NgbActiveModal
  ],
  bootstrap: [AppComponent],
  entryComponents: [
    SpinnerComponent,
    ModalDialogSignInComponent,
    ModalDialogQuestFeedbackComponent,
  ],
})

export class AppModule {
  constructor(public l10nLoader: L10nLoader) {
    // this.l10nLoader.load();
  }
}
