[@react.component]
let make = () => {
  let (isLogin, _setLogin) =
    React.useState(() => {
      let token = Storage.getTokenFromStorage();

      switch (token) {
      | Some(_t) => true
      | None => false
      };
    });
  let (theme, setTheme) = React.useState(() => ThemeContext.Dark);
  let toggleTheme = () =>
    setTheme(current =>
      switch (current) {
      | Light => Dark
      | Dark => Light
      }
    );

  GlobalCss.injectGlobal();
  DesignSystem.Styles.useToggleBodyTheme();
  let url = ReasonReactRouter.useUrl();

  <ThemeContext.Provider value=(theme, toggleTheme)>
    <Header />
    {switch (url.path |> Route.fromUrl, isLogin) {
     | (GitHubCode, false) => <GitHubAuth querystring={url.search} />
     | (_, false) => <Login />
     | (PeopleList, true) => <PeopleList />
     | _ => <div> {React.string("Not found!")} </div>
     }}
  </ThemeContext.Provider>;
};