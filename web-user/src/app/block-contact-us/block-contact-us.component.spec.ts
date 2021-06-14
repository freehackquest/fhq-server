import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { BlockContactUsComponent } from './block-contact-us.component';

describe('BlockContactUsComponent', () => {
  let component: BlockContactUsComponent;
  let fixture: ComponentFixture<BlockContactUsComponent>;

  beforeEach(waitForAsync(() => {
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
