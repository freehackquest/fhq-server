import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { PageNotFoundComponent } from './pages/page-not-found/page-not-found.component';
import { ServerApiComponent } from './pages/server-api/server-api.component';
import { MapActivityComponent } from './pages/map-activity/map-activity.component';
import { NewsComponent } from './pages/news/news.component';
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
import { UserProfileComponent } from './pages/user-profile/user-profile.component';
import { AboutComponent } from './pages/about/about.component';
import { QuestProposalComponent } from './pages/quest-proposal/quest-proposal.component';
import { UserChangePasswordComponent } from './pages/user-change-password/user-change-password.component';
import { ScoreboardComponent } from './pages/scoreboard/scoreboard.component';
import { QuestsComponent } from './pages/quests/quests.component';
import { QuestsBySubjectComponent } from './pages/quests-by-subject/quests-by-subject.component';
import { UserProfileMenuComponent } from './pages/user-profile-menu/user-profile-menu.component';
import { UserLocationComponent } from './pages/user-location/user-location.component';
import { UserSecurityComponent } from './pages/user-security/user-security.component';
import { UserSkillsComponent } from './pages/user-skills/user-skills.component';
import { QuestComponent } from './pages/quest/quest.component';

const routes: Routes = [{
    path: 'knowledge-base',
    component: KnowledgeBaseComponent
  }, {
    path: 'knowledge-base/:id',
    component: KnowledgeBaseComponent
  }, {
    path: 'server-api',
    component: ServerApiComponent
  }, {
    path: 'map-activity',
    component: MapActivityComponent
  }, {
    path: 'news',
    component: NewsComponent
  }, {
    path: 'news/:id',
    component: NewsComponent
  }, {
    path: 'user-profile',
    component: UserProfileComponent
  }, {
    path: 'user-security',
    component: UserSecurityComponent
  }, {
    path: 'user-skills',
    component: UserSkillsComponent
  }, {
    path: 'user-profile-menu',
    component: UserProfileMenuComponent
  }, {
    path: 'user-change-password',
    component: UserChangePasswordComponent
  }, {
    path: 'about',
    component: AboutComponent
  }, {
    path: 'chat',
    component: ChatComponent
  }, {
    path: 'automation-ussr',
    component: AutomationUssrComponent
  }, {
    path: 'automation-ussr-presentation',
    component: AutomationUssrPresentationComponent
  }, {
    path: 'automation-ussr-rating',
    component: AutomationUssrRatingComponent
  }, {
    path: 'registration',
    component: RegistrationComponent
  }, {
    path: 'reset-password',
    component: ResetPasswordComponent
  }, {
    path: 'donate',
    component: DonateComponent
  }, {
    path: 'useful-links',
    component: UsefulLinksComponent
  }, {
    path: 'games',
    component: GamesComponent
  }, {
    path: 'games/:id',
    component: GamesComponent
  }, {
    path: 'confidencial',
    component: ConfidencialComponent
  }, {
    path: 'feedback',
    component: FeedbackComponent
  }, {
    path: 'quest-proposal',
    component: QuestProposalComponent
  }, {
    path: 'scoreboard',
    component: ScoreboardComponent
  } , {
      path: 'quests',
      component: QuestsComponent
  }, {
      path: 'quests/:subject',
      component: QuestsBySubjectComponent
  }, {
      path: 'quest/:id',
      component: QuestComponent
  }, {
    path: '**',
    component: PageNotFoundComponent
  }
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
