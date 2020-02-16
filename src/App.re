[@react.component]
let make = () => {
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
    {switch (url.path |> Route.fromUrl, Storage.getTokenFromStorage()) {
     | (GitHubCode, _) => <GitHubAuth querystring={url.search} />
     | (_, None) => <Login />
     | (PeopleList, Some(_)) => <PeopleList />
     | _ => <div> {React.string("Not found!")} </div>
     }}
  </ThemeContext.Provider>;
};