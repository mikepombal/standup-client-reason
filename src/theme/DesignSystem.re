module Tokens = {
  type backgroundColor = [ | `BodyBg1 | `BodyBg2 | `InputBg | `CardBg];

  type textColor = [ | `BodyText | `SecondaryText];

  type color = [
    | `Primary
    | `PrimaryQuiet
    | `Secondary
    | `Neutral
    | `Placeholder
    | `Error
    | `Disabled
    | backgroundColor
    | textColor
  ];

  type fontVariant = [ | `xs | `sm | `md | `lg | `xl | `xxl | `base];

  type fontFamily = [ | `base | `heading];

  type spacing = [
    | `xxs
    | `xs
    | `sm
    | `md
    | `lg
    | `xl
    | `xxl
    | `custom(int)
  ];

  type transitions = [ | `modal | `component | `theme];

  type borderRadius = [ | `base | `lg | `xl];
};

module Theme = {
  open Colors;

  /** base values */
  let baseFontSizePx = 20;
  let baseLineHeight = 1.4; // 28px
  let headingLineHeight = 1.2;

  let baseLineGridPx = 4; // factor of 28px
  let fontScale = 1.2;

  let fontSize = factor =>
    fontScale
    ** (factor |> float_of_int)
    *. (baseFontSizePx |> float_of_int)
    |> int_of_float;

  let lineHeight =
    fun
    | `body => baseLineHeight
    | `heading => headingLineHeight;

  let fontVariant = (token: Tokens.fontVariant) => {
    let variant = (factor, line) => (fontSize(factor), lineHeight(line));

    switch (token) {
    | `xs => variant(-2, `body)
    | `sm => variant(-1, `body)
    | `base => variant(0, `body)
    | `md => variant(1, `body)
    | `lg => variant(2, `heading)
    | `xl => variant(3, `heading)
    | `xxl => variant(4, `heading)
    };
  };

  let space =
    fun
    | `xxs => baseLineGridPx
    | `xs => baseLineGridPx * 2
    | `sm => baseLineGridPx * 3
    | `md => baseLineGridPx * 4
    | `lg => baseLineGridPx * 6
    | `xl => baseLineGridPx * 8
    | `xxl => baseLineGridPx * 10
    | `custom(multiplier) => multiplier * baseLineGridPx;

  let lightPalette = (token: Tokens.color) => {
    switch (token) {
    | `Primary => Green.main
    | `PrimaryQuiet => Green.dark1
    | `Secondary => DarkBlue.light4
    | `Neutral => Gray.light1
    | `Placeholder => Gray.dark1
    | `Error => Red.main
    | `Disabled => Gray.main
    | `BodyBg1 => White.main
    | `BodyBg2 => DarkBlue.light5
    | `CardBg => White.main
    | `InputBg => White.main
    | `BodyText => DarkBlue.dark1
    | `SecondaryText => DarkBlue.light1
    };
  };

  let darkPalette = (token: Tokens.color) => {
    switch (token) {
    | `Primary => Green.main
    | `PrimaryQuiet => Green.light1
    | `Secondary => DarkBlue.light1
    | `Neutral => Gray.light1
    | `Placeholder => Gray.dark1
    | `Error => Red.main
    | `Disabled => Gray.dark2
    | `BodyBg1 => DarkBlue.dark2
    | `BodyBg2 => DarkBlue.dark2
    | `CardBg => DarkBlue.dark1
    | `InputBg => DarkBlue.dark1
    | `BodyText => DarkBlue.light5
    | `SecondaryText => DarkBlue.light4
    };
  };

  let fontFamily =
    fun
    | `base => "Rubik, sans-serif"
    | `heading => "Muli, system-ui, sans-serif";

  let borderRadius =
    fun
    | `base => 4
    | `lg => 8
    | `xl => 20;

  type transition = {
    duration: int,
    fn: Css_AtomicTypes.TimingFunction.t,
  };

  let transition =
    fun
    | `modal => {duration: 300, fn: Css.cubicBezier(0.77, 0.0, 0.175, 1.0)}
    | `component => {duration: 150, fn: Css.easeInOut}
    | `theme => {duration: 200, fn: Css.easeInOut};
};

module Styles = {
  let font = variant =>
    switch (variant |> Theme.fontVariant) {
    | (font_size, line_height) =>
      Css.[
        fontSize(`px(font_size)),
        lineHeight(`abs(line_height)),
        fontFamily(Theme.fontFamily(`base)),
      ]
    };

  let space = (token: Tokens.spacing) => `px(token |> Theme.space);

  let color = (token, theme: ThemeContext.theme) => {
    switch (theme) {
    | Dark =>
      switch (token) {
      | `Overlay => Colors.Overlay.light
      | #Tokens.color as c => `hex(c |> Theme.darkPalette)
      }
    | Light =>
      switch (token) {
      | `Overlay => Colors.Overlay.dark
      | #Tokens.color as c => `hex(c |> Theme.lightPalette)
      }
    };
  };

  let borderRadius = (token: Tokens.borderRadius) =>
    Css.borderRadius(`px(token |> Theme.borderRadius));

  let paddingV = token => [
    token |> space |> Css.paddingTop,
    token |> space |> Css.paddingBottom,
  ];

  let paddingH = token => [
    token |> space |> Css.paddingLeft,
    token |> space |> Css.paddingRight,
  ];

  let animation = (token, name) => {
    let transition = token |> Theme.transition;
    Css.animation(
      ~duration=transition.duration,
      ~timingFunction=transition.fn,
      name,
    );
  };

  let transition = (token, property) => {
    let value = token |> Theme.transition;
    Css.(
      transition(~duration=value.duration, ~timingFunction=value.fn, property)
    );
  };

  let injectGlobal = () => {
    Css.global(
      "body",
      [transition(`theme, "background-color, color"), ...font(`base)],
    );

    Css.global(
      ".dark-theme",
      [
        Css.backgroundColor(color(`BodyBg1, Dark)),
        Css.color(color(`BodyText, Dark)),
      ],
    );

    Css.global(
      ".light-theme",
      [
        Css.backgroundColor(color(`BodyBg1, Light)),
        Css.color(color(`BodyText, Light)),
      ],
    );

    Css.(
      global(
        ".block-scroll",
        [width(pct(100.0)), height(pct(100.0)), overflow(hidden)],
      )
    );
  };

  let useColor = (token: [ Tokens.color | `Overlay]) => {
    let (theme, _) = ThemeContext.useTheme();
    color(token, theme);
  };

  let useToggleBodyTheme = () => {
    let (theme, _) = ThemeContext.useTheme();
    switch (theme) {
    | Dark =>
      DomUtils.addBodyClass("dark-theme");
      DomUtils.removeBodyClass("light-theme");
    | Light =>
      DomUtils.addBodyClass("light-theme");
      DomUtils.removeBodyClass("dark-theme");
    };
  };
};