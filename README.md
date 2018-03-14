# ReasonSplash

[Unplash](https://unsplash.com/) mobile client built with [ReasonML](https://reasonml.github.io/)

## Getting Started
1. Install [Bucklescript](https://bucklescript.github.io/) globally which will compile your Reason to JavaScript
   ```
   yarn global add bs-platform
   ```
2. Install dependencies
   ```
   yarn
   ```
3. Create an Environment module from the template, replacing the placeholder Unsplash application ID and secret with your own
   ```
   cp re/Environment.re.sample re/Environment.re
   ```

4. Start the app
   ```
   yarn start
   ```
5. Watch for file changes in a separate terminal
   ```
   yarn run watch
   ```

![Demo](demo.gif?raw=true "Demo")
