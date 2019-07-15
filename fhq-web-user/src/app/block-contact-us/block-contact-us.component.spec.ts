import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { BlockContactUsComponent } from './block-contact-us.component';

describe('BlockContactUsComponent', () => {
  let component: BlockContactUsComponent;
  let fixture: ComponentFixture<BlockContactUsComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ BlockContactUsComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(BlockContactUsComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
