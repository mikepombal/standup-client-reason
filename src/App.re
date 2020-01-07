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

  <ThemeContext.Provider value=(theme, toggleTheme)>
    <Header />
    {isLogin ? <PeopleList /> : <Login />}
  </ThemeContext.Provider>;
};