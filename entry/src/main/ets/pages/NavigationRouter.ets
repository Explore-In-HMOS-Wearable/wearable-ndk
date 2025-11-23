import { Index } from './Index';
import { MenuPage } from './MenuPage';
import { SplashPage } from './SplashPage';

@Entry
@Component
struct NavigationRouter {
  @Provide('pageInfos') pageInfos: NavPathStack = new NavPathStack()

  @Builder
  routerMap(builderName: string, param: object) {
    if (builderName === 'Index') {
      Index()
    } else if (builderName === 'MenuPage') {
      MenuPage()
    } else if (builderName === 'SplashPage') {
      SplashPage()
    }
  }

  aboutToAppear() {
    this.pageInfos.pushPathByName('SplashPage', null);
  }

  build() {
    Navigation(this.pageInfos) {
    }
    .mode(NavigationMode.Stack)
    .navDestination(this.routerMap)
    .hideNavBar(true)

  }
}

