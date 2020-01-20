# Introduction and reference infos
mkdocs works by installing mkdocs python's packages and a bunch of python markdown extensions

- [standard markdown syntax](https://www.markdownguide.org/).
- For full documentation visit [mkdocs.org](https://mkdocs.org). To install mkdocs

    `python -m pip install mkdocs`

- There are a list of bundles [markdown extensions](https://facelessuser.github.io/pymdown-extensions/):

    `python -m pip install pymdown-extensions`

- There are a list of third party [markdown extensions](https://github.com/Python-Markdown/markdown/wiki/Third-Party-Extensions). Interesting extensions are :

    - [markdown-katex](https://gitlab.com/mbarkhau/markdown-katex)
    - [plantuml-markdown](https://github.com/mikitex70/plantuml-markdown)


Once everything is installed, a project can be created through [mkdocs getting started guide](https://www.mkdocs.org/#getting-started). Configurations are done in mkdocs.yml file. All the rest are just *.md files

Note that for mathematical formulas, the default usage is to use MathJAX functionalities, which is by default using arithmatex. In future, katex might be prefered