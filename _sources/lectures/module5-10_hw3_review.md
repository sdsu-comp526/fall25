# 25) HW3 Review

## Today
- 1. Review of HW3
- 2. Submission expectations and reminders

## 1. Review of HW3

Solution to HW3. Here is my C code:

```{literalinclude} ../c_programs/module5-10_hw3_review/dot.c
:language: c
:linenos: false
```

This program can be compiled with the GNU C compiler with:

```
gcc dot.c -lm -o dot
```

and executed with

```
./dot
```

### Common mistakes

Here is a list of common mistakes that a few people made:
- Part 4: Not really implementing a blocked dot product between the rows of $M$ and the columns of $N$. The best way would have been reusing the `dot_product` function that was implemented for Part 1. This allows for code re-use, which a good practice.
- E.C.: not safely initialized to zero the output variable for the triple nested loop matrix-matrix multiplication. You could have achieved this by either invoking `calloc` instead of `malloc` or by manually initialized to zero all entries of the output variable (typically this is done right before the inner-most loop).

## 2. Submission expectations and reminders

- In general, when you receive an Assignment via a GitHub Classroom link, you want to clone your assignment repository, by doing

```shell
git clone your_assignment_repository_url
```

- You can also work on a back-up repository or directory of your choice if you want to, for your scrap work, but you _have to_ clone the assignment repository and submit your work there to be considered for submission and grading.

- As soon as you clone your Assignment repository, move to that repository

```bash
cd your_assignment_repository
```

- Create a new feature branch and switch to that. You can do this in two ways:
  * `git checkout -b name_of_your_branch`
  * `git branch name_of_your_branch` and then `git checkout name_of_your_branch`

- Do **NOT** work directly off `main`

- You can work on your feature branch as much as you like and create repeated incremental snapshots of your work via `git commit`. Always remember to use meaningful commit messages to remind yourself (and others) about your work in that moment in time. In a terminal you can simply do this by

```bash
git commit -m "Your commit message"
```

You can also write multi-line more detailed commit messages if you want. Just simply separate them with a space, and repeat the `-m` option, as in:

```bash
git commit -m "Your commit message" -m "Your more detailed message on a new line"
```

- When you are satisfied with your committed work, you can push it to your working branch via:

```bash
git push origin your_branch_name
```

If it is the first time you are doing this, `git` will automatically tell you that you can open a Pull Request with your changes. Just CTRL-click on the URL that git shows you in the terminal and you will be sent to your Pull Request web interface.

Any successive changes that you want to push to your branch, they will be automatically reflected on the open PR.

- Only changes made within the deadline (including the lateness window) will be graded.

- Remember not to attempt to close or merge your PR without any Reviewer (in this case your instructor) approval.

- Always remember to double check the `File changed` tab in your PR. If you see files that should not belong there (e.g., files automatically created by your IDE or virtual environment files) remove them.

- If you are using an IDE that automatically creates hidden project files that you might inadvertently push to your branch, it is always a good practice to use a `.gitignore` file that specify which files you do _not_ want to be tracked by `git`, and therefore, pushed to your branch. Recall that we covered this in our [first lecture](https://sdsu-comp526.github.io/fall24/slides/module1-1_first_class.html#keeping-track-with-git).

### Reminder about the AI policy in this course

A friendly reminder that in this course, we follow the University Senate's extended definition of plagiarism that includes the un-cited use of generative AI applications, specifically: “representing work produced by generative Artificial Intelligence as one’s own.”  

I provided in the [Syllabus](https://sdsu-comp526.github.io/fall25/syllabus.html#use-of-ai) examples of how to properly cite the use of any genAI or LLM tool.