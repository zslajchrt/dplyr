% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/colwise-group-by.R
\name{group_by_all}
\alias{group_by_all}
\alias{group_by_at}
\alias{group_by_if}
\title{Group by a selection of variables}
\usage{
group_by_all(.tbl, .funs = list(), ...)

group_by_at(.tbl, .vars, .funs = list(), ..., .add = FALSE)

group_by_if(.tbl, .predicate, .funs = list(), ..., .add = FALSE)
}
\arguments{
\item{.tbl}{A \code{tbl} object.}

\item{.funs}{List of function calls generated by \code{\link[=funs]{funs()}}, or a
character vector of function names, or simply a function.

Bare formulas are passed to \code{\link[rlang:as_function]{rlang::as_function()}} to create
purrr-style lambda functions. Note that these lambda prevent
hybrid evaluation from happening and it is thus more efficient to
supply functions like \code{mean()} directly rather than in a
lambda-formula.}

\item{...}{Additional arguments for the function calls in
\code{.funs}. These are evaluated only once, with \link[rlang:tidy-dots]{tidy dots} support.}

\item{.vars}{A list of columns generated by \code{\link[=vars]{vars()}},
a character vector of column names, a numeric vector of column
positions, or \code{NULL}.}

\item{.add}{Passed to the \code{add} argument of \code{\link[=group_by]{group_by()}}.}

\item{.predicate}{A predicate function to be applied to the columns
or a logical vector. The variables for which \code{.predicate} is or
returns \code{TRUE} are selected. This argument is passed to
\code{\link[rlang:as_function]{rlang::as_function()}} and thus supports quosure-style lambda
functions and strings representing function names.}
}
\description{
These \link{scoped} variants of \code{\link[=group_by]{group_by()}} group a data frame by a
selection of variables. Like \code{\link[=group_by]{group_by()}}, they have optional
\link{mutate} semantics.
}
\examples{
# Group a data frame by all variables:
group_by_all(mtcars)

# Group by variables selected with a predicate:
group_by_if(iris, is.factor)

# Group by variables selected by name:
group_by_at(mtcars, vars(vs, am))

# Like group_by(), the scoped variants have optional mutate
# semantics. This provide a shortcut for group_by() + mutate():
d <- tibble(x=c(1,1,2,2), y=c(1,2,1,2))
group_by_all(d, as.factor)
group_by_if(iris, is.factor, as.character)
}
